//
// Created by Christopher Boyle on 12/10/2021.
//

#include "Renderable.h"

Renderable::Renderable(Object *parent)
    :   Object(parent)
{
    add_type("Renderable");
}

void Renderable::render()
{
    render_me();
    set_not_moved();
}

void Renderable::after_children_rendered()
{
    (void) this; // do nothing
}