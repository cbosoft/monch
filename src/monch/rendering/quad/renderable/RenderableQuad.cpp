//
// Created by Christopher Boyle on 17/10/2021.
//

#include <monch/rendering/renderer/Renderer.h>
#include "RenderableQuad.h"

RenderableQuad::RenderableQuad(Object *parent)
    : Renderable(parent)
    , Quad(this)
{
    Renderer::ref().assign_shader(this, "default");
}

void RenderableQuad::render()
{
    Renderer::ref().use_assigned_shader(this);
    draw();
}