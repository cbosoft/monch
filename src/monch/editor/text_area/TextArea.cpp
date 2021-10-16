//
// Created by Christopher Boyle on 13/10/2021.
//

#include <iostream>

#include <monch/editor/rendered_character/RenderedCharacter.h>
#include "TextArea.h"

TextArea::TextArea(Object *parent, int w, int h)
    :   Container(parent, w, h)
    ,   _cursor_position{0}
    ,   _font{nullptr}
    ,   _topleft{nullptr}
{
    add_type<TextArea>();
    _font = FontManager::ref().get_font("iosevka-regular.ttf", 16);
    _cursor = new RenderedCharacter(this, U'\u258f', _font);
    _topleft = new Object(this);
    _topleft->set_relative_position({0, h - 20});
    reposition_cursor();
}

void TextArea::increment_cursor_position()
{
    if (_cursor_position < _rendered_characters.size()) {
        _cursor_position++;
        reposition_cursor();
    }
}

void TextArea::set_size(int w, int h)
{
    Container::set_size(w, h);
    _topleft->set_relative_position({0, h - 20});
}

void TextArea::decrement_cursor_position()
{
    if (_cursor_position > 0) {
        _cursor_position--;
        reposition_cursor();
    }
}

void TextArea::reposition_cursor()
{
    if (_cursor_position) {
        _cursor->set_parent(_rendered_characters[_cursor_position-1]);
        _cursor->set_relative_position({_rendered_characters[_cursor_position-1]->get_advance(), 0});
    }
    else {
        _cursor->set_parent(this);
        _cursor->set_relative_position({0, get_height() - 20});
    }
}

void TextArea::add_char(char32_t c)
{
    Object *root = _topleft;
    if (!_rendered_characters.empty()) root = _rendered_characters[_cursor_position-1];

    auto *rchar = new RenderedCharacter(root, c, _font);

    if (root->is_a<RenderedCharacter>()) {
        rchar->set_relative_position({((RenderedCharacter *)root)->get_advance(), 0});
    }

    if (_cursor_position < (int(_rendered_characters.size()) - 1)) {
        _rendered_characters[_cursor_position]->insert_parent(rchar);
        // update spacing
    }

    _text.insert(_text.begin() + _cursor_position, c);
    _rendered_characters.insert(_rendered_characters.begin()+_cursor_position, rchar);
    increment_cursor_position();
}

void TextArea::backspace()
{
    if (!_cursor_position) {
        // nothing to delete
        return;
    }

    // deletion in the middle of the text: join up parentage of the survivng nodes
    if (_cursor_position < (_text.size() - 1)) {
        RenderedCharacter *left = _rendered_characters[_cursor_position-2];
        RenderedCharacter *right = _rendered_characters[_cursor_position];
        right->set_parent(left);
    }

    // delete to left
    _text.erase(_text.begin() + (_cursor_position - 1));
    auto it_char = _rendered_characters.begin() + (_cursor_position - 1);
    auto *rchar = *it_char;
    _rendered_characters.erase(it_char);
    delete rchar;

    decrement_cursor_position();
}

void TextArea::newline()
{
    auto *nl = new RenderedCharacter(this, ' ', _font);
    Object *root = this;
    if (_cursor_position) root = _rendered_characters[_cursor_position-1];
    nl->set_relative_position<TextArea>({0, root->get_position().y-20});
    _text.insert(_text.begin() + _cursor_position, '\n');
    _rendered_characters.insert(_rendered_characters.begin()+_cursor_position, nl);

    increment_cursor_position();
}