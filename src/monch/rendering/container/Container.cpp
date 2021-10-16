//
// Created by Christopher Boyle on 14/10/2021.
//

#include <iostream>
#include <fstream>

#include <monch/rendering/gl.h>
#include <monch/rendering/renderer/Renderer.h>
#include "Container.h"

Container::Container(Object *parent, int w, int h)
    :   Renderable(parent)
    ,   _r{0}
    ,   _g{0}
    ,   _b{0}
    ,   _w{w}
    ,   _h{h}
    ,   _vbuff{this, 4}
    ,   _fbuff{0}
    ,   _txtr{0}
{
    add_type<Container>();
    glGenFramebuffers(1, &_fbuff);
    glGenTextures(1, &_txtr);
    Renderer::ref().assign_shader(this, "texture");
    set_relative_position({0, 0});
    _vbuff.set_tex_coords({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
}

void Container::set_size(int w, int h)
{
    _w = w;
    _h = h;
    invalidate_position_scale();
}

int Container::get_width() const { return _w; }
int Container::get_height() const { return _h; }

void Container::render()
{
    int W = _w*2, H = _h*2;

    // change render target: texture
    glBindFramebuffer(GL_FRAMEBUFFER, _fbuff);
    glBindTexture(GL_TEXTURE_2D, _txtr);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W, H, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _txtr, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("error setting up fbuff");
    glViewport(0, 0, W, H);
    glClearColor(_r, _g, _b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Container::after_children_rendered()
{
//    int W=_w*2, H=_h*2;
//    std::vector<unsigned char> pixels(W*H*3);
//    glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
//
//    std::string opath = get_id()+ ".ppm";
//    std::ofstream of(opath);
//    of << "P3\n# foo\n" << W << " " << H << "\n255\n";
//
//    int k = 0;
//    for(int i = 0; i < W; i++)
//    {
//        for(int j = 0; j < H; j++)
//        {
//            of << (unsigned int)pixels[k] << " " << (unsigned int)pixels[k+1] << " " << (unsigned int)pixels[k+2] << " ";
//            k = k+3;
//        }
//        of << "\n";
//    }

    // change back to on-screen rendering, or render to parent fbuff
    if (has_parent()) {
        auto *c = find_in_parents<Container>();
        glBindFramebuffer(GL_FRAMEBUFFER, c->_fbuff);
        glViewport(0, 0, c->_w*2, c->_h*2);
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // render a quad to the screen
    glBindTexture(GL_TEXTURE_2D, _txtr);
    Renderer::ref().use_assigned_shader(this);
    if (has_invalid_position_scale()) {
        WindowPoint pt = {0, 0};
        if (has_parent()) {
            pt = get_relative_position<Container>();
        }
        else {
            pt = get_absolute_position();
        }
        _vbuff.set_points({{pt.x, pt.y}, {pt.x+_w, pt.y}, {pt.x+_w, pt.y+_h}, {pt.x, pt.y+_h}});
    }
    _vbuff.draw();
}

void Container::set_colour(float r, float g, float b)
{
    _r = r; _b = b; _g = g;
}

WindowPoint Container::get_size() const
{
    return {_w, _h};
}

NormalisedPoint Container::get_norm(const WindowPoint &pt) const
{
    // map pixel point (x, y) to range ([-1, 1], [-1, 1])
    auto nx = float(pt.x*2)/float(_w) - 1.f;
    auto ny = float(pt.y*2)/float(_h) - 1.f;
    return {nx, ny};
}