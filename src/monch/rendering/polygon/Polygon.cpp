//
// Created by Christopher Boyle on 12/10/2021.
//

#include <iostream>

#include <monch/rendering/renderable/Renderable.h>
#include <monch/rendering/renderer/Renderer.h>
#include <monch/editor/app/EditorApp.h>
#include <monch/rendering/gl.h>

#include "Polygon.h"


Polygon::Polygon(Renderable *owner, uint n)
    :   _id{0}
    ,   _va{0}
    ,   _unscaled(n)
    ,   _owner{owner}
    ,   _is_init{false}
    ,   _sync_needed{true}
{
    for (uint i = 0; i < n; i++) {
        _vertices.push_back({.x=0, .y=0, .z=0., .r=1., .g=1., .b=1., .a=1., .s=0., .t=0.});
    }
}

void Polygon::init()
{
    Renderer::ref().error_check("vbuff ctor (start)");
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
    Renderer::ref().error_check("vbuff ctor");
}

Polygon::~Polygon()
{
    uint buffers[1] = {_id};
    glDeleteBuffers(1, buffers);
}


void Polygon::set_points(const std::vector<WindowPoint> &points)
{
    if (points.size() != _vertices.size()) return;
    _unscaled = points;
    rescale();
}


void Polygon::set_colour(float r, float g, float b, float a)
{
    for (auto &vertex : _vertices) {
        vertex.r = r;
        vertex.g = g;
        vertex.b = b;
        vertex.a = a;
    }
    _sync_needed = true;
}


void Polygon::set_tex_coords(const std::vector<NormalisedPoint> &points)
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


void Polygon::sync_gl()
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, long(_vertices.size()*sizeof(Vertex)), &_vertices[0], GL_DYNAMIC_DRAW);
    _sync_needed = false;

    Renderer::ref().error_check("Polygon::sync_gl()");
}

void Polygon::draw()
{
    if (!_is_init) init();
    if (_sync_needed) sync_gl();
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBindVertexArray(_va);
    glDrawArrays(GL_TRIANGLE_FAN, 0, int(_vertices.size()));

    Renderer::ref().error_check("Polygon::draw()");
}

void Polygon::rescale()
{
    auto it_v = _vertices.begin();
    auto it_p = _unscaled.begin();
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

        NormalisedPoint n_pt = c->get_norm(pt);
        vertex.x = n_pt.x;
        vertex.y = n_pt.y;
    }
    _sync_needed = true;
}