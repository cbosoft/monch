//
// Created by Christopher Boyle on 13/10/2021.
//

#include <iostream>
#include <monch/rendering/container/Container.h>
#include "RenderedCharacter.h"

RenderedCharacter::RenderedCharacter(Object *parent, char32_t ch, Font *font)
        :   Renderable(parent)
        ,   _texture_id{0}
        ,   _w{1}
        ,   _h{1}
        ,   _advance{1}
        ,   _font{font}
        ,   _char{ch}
{
    add_type<RenderedCharacter>();
    Renderer::ref().assign_shader(this, "font");
    Character *c = font->get_char(ch);
    _w = c->w;
    _h = c->h;
    _bearing_x = c->bearing_x;
    _bearing_y = c->bearing_y;
    _advance = c->advance;
    _texture_id = c->texture_id;
    _vbuff = new VertexBuffer(this, 4);
    _vbuff->set_tex_coords({{0, 1}, {1, 1}, {1, 0}, {0, 0}});
}

RenderedCharacter::~RenderedCharacter() noexcept
{
    delete _vbuff;
}

int RenderedCharacter::get_advance() const
{
    return _advance;
}

void RenderedCharacter::render_me()
{
    Renderer::ref().use_assigned_shader(this);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    if (has_invalid_position_scale()) {
        update_vbuff_points();
    }
    _vbuff->draw();
}

void RenderedCharacter::update_vbuff_points()
{
    WindowPoint pt = {0, 0};
    if (has_parent()) {
        pt = get_relative_position<Container>();
    }
    else {
        pt = get_position();
    }
    int x = pt.x + _bearing_x, y = pt.y + _bearing_y - _h;
    _vbuff->set_points({ {x, y}, {x + _w, y}, {x + _w, y + _h}, {x, y + _h}});
}