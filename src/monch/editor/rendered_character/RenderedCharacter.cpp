//
// Created by Christopher Boyle on 13/10/2021.
//

#include <iostream>
#include <monch/rendering/container/Container.h>
#include "RenderedCharacter.h"

RenderedCharacter::RenderedCharacter(Object *parent, char32_t ch, Font *font)
        :   Renderable(parent)
        ,   _texture_id{0}
        ,   _advance{1}
        ,   _font{font}
        ,   _char{ch}
{
    add_type<RenderedCharacter>();
    Renderer::ref().assign_shader(this, "font");
    Character *c = font->get_char(ch == U'\n' ? U' ' : ch);
    Renderable::set_size(c->w, c->h);
    _bearing_x = c->bearing_x;
    _bearing_y = c->bearing_y;
    _advance = c->advance;
    _texture_id = c->texture_id;
    _quad = new Quad(this);
    _quad->set_tex_coords({NormalisedPoint{0, 1}, NormalisedPoint{1, 1}, NormalisedPoint{1, 0}, NormalisedPoint{0, 0}});
    _quad->set_offset({_bearing_x, _bearing_y - get_height()});
}

RenderedCharacter::~RenderedCharacter() noexcept
{
    delete _quad;
}

int RenderedCharacter::get_advance() const
{
    return _advance;
}

void RenderedCharacter::render()
{
    Renderer::ref().use_assigned_shader(this);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    _quad->draw();
}

char32_t RenderedCharacter::get_char() const
{
    return _char;
}