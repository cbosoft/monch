//
// Created by Christopher Boyle on 14/10/2021.
//

#include <iostream>
#include <fstream>

#include <monch/rendering/gl.h>
#include <monch/rendering/renderer/Renderer.h>
#include "Container.h"

Container::Container(Object *parent, int w, int h)
    : Renderable(parent)
    , _r{0}
    , _g{0}
    , _b{0}
    , _quad{this}
    , _fbuff{0}
    , _txtr{0}
{
    add_type<Container>();
    Object::set_size(w, h);
    glGenFramebuffers(1, &_fbuff);
    glGenTextures(1, &_txtr);
    Renderer::ref().assign_shader(this, "texture");
    set_relative_position({0, 0});
    _quad.set_tex_coords({NormalisedPoint{0, 0}, NormalisedPoint{1, 0}, NormalisedPoint{1, 1}, NormalisedPoint{0, 1}});
}

void Container::render()
{
    WindowPoint sz = get_size();
    int W = sz.x*2, H = sz.y*2;

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
    if (has_container()) {
        auto *c = get_container();
        glBindFramebuffer(GL_FRAMEBUFFER, c->_fbuff);
        WindowPoint sz = c->get_size();
        int W = sz.x*2, H = sz.y*2;
        glViewport(0, 0, W, H);
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // render a quad to the screen
    glBindTexture(GL_TEXTURE_2D, _txtr);
    Renderer::ref().use_assigned_shader(this);
    _quad.draw();
}

void Container::set_colour(float r, float g, float b)
{
    _r = r; _b = b; _g = g;
}

NormalisedPoint Container::get_norm(const WindowPoint &pt) const
{
    // map pixel point (x, y) to range ([-1, 1], [-1, 1])
    WindowPoint sz = get_size();
    auto nx = float(pt.x*2)/float(sz.x) - 1.f;
    auto ny = float(pt.y*2)/float(sz.y) - 1.f;
    return {nx, ny};
}