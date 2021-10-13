//
// Created by Christopher Boyle on 13/10/2021.
//

#include <monch/editor/rendered_character/RenderedCharacter.h>
#include "TextArea.h"

TextArea::TextArea(Object *parent)
    : Renderable(parent)
    ,   _cursor_position{0}
    ,   _font{nullptr}
{
    add_type("text area");
    _font = FontManager::ref().get_font("iosevka-regular.ttf", 16);
}

void TextArea::add_char(char32_t c)
{
    _text.insert(_text.begin() + _cursor_position, c);

    Object *root = this;
    if (!_rendered_characters.empty()) root = _rendered_characters[_cursor_position-1];

    auto *rchar = new RenderedCharacter(root, c, _font);
    _rendered_characters.insert(_rendered_characters.begin()+_cursor_position, rchar);
    _cursor_position ++;

    if (root->is_a("RenderedCharacter")) {
        rchar->set_relative_position({((RenderedCharacter *)root)->get_advance(), 0});
    }
    // TODO, what if char to right?
    // update relpositions and parentage
}

void TextArea::render_me()
{
    // do nothing (rendering is handled in characters)
}