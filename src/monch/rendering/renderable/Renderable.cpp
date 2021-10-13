//
// Created by Christopher Boyle on 12/10/2021.
//

#include "Renderable.h"

Renderable::Renderable(Object *parent)
    :   Object(parent)
{
    add_type("renderable");
}

void Renderable::render()
{
    render_me();
    render_children();
    after_children_rendered();
    set_not_moved();
}

void Renderable::render_children()
{
    for (auto *child : *this)
    {
        if (child->is_a("renderable")) {
            ((Renderable *) child)->render();
        }
    }
}

void Renderable::after_children_rendered()
{
    (void) this; // do nothing
}