//
// Created by Christopher Boyle on 12/10/2021.
//

#include <monch/rendering/renderer/Renderer.h>
#include "Quad.h"

Quad::Quad(Object *parent, int x, int y, int w, int h)
    :   Renderable(parent)
    ,   x{x}
    ,   y{y}
    ,   w{w}
    ,   h{h}
    ,   _vbuff(this, 4)
{
    Renderer::ref().assign_shader(this, "default");
}

void Quad::render()
{
    Renderer::ref().use_assigned_shader(this);
    _vbuff.set_points({{x, y}, {x+w, y}, {x+w, y+h}, {x, y+h}});
    _vbuff.draw();
}

void Quad::set_colour(float r, float g, float b)
{
    _vbuff.set_colour(r, g, b, 1.0);
}