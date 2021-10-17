//
// Created by Christopher Boyle on 12/10/2021.
//

#include <iostream>

#include <monch/rendering/renderable/Renderable.h>
#include <monch/rendering/renderer/Renderer.h>
#include <monch/editor/app/EditorApp.h>
#include <monch/rendering/gl.h>

#include "Quad.h"


Quad::Quad(Renderable *owner)
    :   _id{0}
    ,   _va{0}
    ,   _owner{owner}
    ,   _is_init{false}
    ,   _sync_needed{true}
    ,   _offset({0, 0})
{
    _vertices[0] = {.x=0, .y=0, .z=0., .r=1., .g=1., .b=1., .a=1., .s=0., .t=0.};
    _vertices[1] = {.x=0, .y=0, .z=0., .r=1., .g=1., .b=1., .a=1., .s=0., .t=0.};
    _vertices[2] = {.x=0, .y=0, .z=0., .r=1., .g=1., .b=1., .a=1., .s=0., .t=0.};
    _vertices[3] = {.x=0, .y=0, .z=0., .r=1., .g=1., .b=1., .a=1., .s=0., .t=0.};
}

void Quad::init()
{
    Renderer::ref().error_check("quad init (start)");
    glGenBuffers(1, &_id);
    sync_gl();
    glGenVertexArrays(1, &_va);
    glBindVertexArray(_va);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, x)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, r)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, s)));
    glEnableVertexAttribArray(2);
    Renderer::ref().error_check("quad init");
}

Quad::~Quad()
{
    uint buffers[1] = {_id};
    glDeleteBuffers(1, buffers);
}


void Quad::set_colour(float r, float g, float b, float a)
{
    for (auto &vertex : _vertices) {
        vertex.r = r;
        vertex.g = g;
        vertex.b = b;
        vertex.a = a;
    }
    _sync_needed = true;
}


void Quad::set_tex_coords(const std::array<NormalisedPoint, 4> &points)
{
    if (points.size() != _vertices.size()) return;

    auto it_v = _vertices.begin();
    auto it_p = points.begin();
    for (; it_v != _vertices.end(); it_v++, it_p++) {
        auto &vertex = *it_v;
        auto tex_coord = *it_p;

        vertex.s = tex_coord.x;
        vertex.t = tex_coord.y;
    }
    _sync_needed = true;
}


void Quad::sync_gl()
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, long(_vertices.size()*sizeof(Vertex)), &_vertices[0], GL_DYNAMIC_DRAW);
    _sync_needed = false;

    Renderer::ref().error_check("VertexBuffer::sync_gl()");
}


void Quad::draw()
{
    if (!_is_init) init();
    if (_owner->has_invalid_position_scale()) rescale();
    if (_sync_needed) sync_gl();
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBindVertexArray(_va);
    glDrawArrays(GL_TRIANGLE_FAN, 0, int(_vertices.size()));

    Renderer::ref().error_check("VertexBuffer::draw()");
}


void Quad::rescale()
{
    auto it_v = _vertices.begin();
    WindowPoint pos = _owner->get_position(), size = _owner->get_size();
    std::list<WindowPoint> points = {
            pos,
            {pos.x+size.x, pos.y},
            pos+size,
            {pos.x, pos.y+size.y}
    };
    auto it_p = points.begin();
    Container *c = nullptr;
    if (_owner->has_container()) {
        c = _owner->get_container();
    }
    else {
        // no container; obj is its own container
        c = (Container *)_owner;
    }

    for (; it_v != _vertices.end(); it_v++, it_p++) {
        auto &vertex = *it_v;
        const auto &pt = *it_p;

        NormalisedPoint n_pt = c->get_norm(pt+_offset);
        vertex.x = n_pt.x;
        vertex.y = n_pt.y;
    }
    _sync_needed = true;
}

void Quad::set_offset(const WindowPoint &off)
{
    _offset = off;
}