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
    ,   _min_w{w}
    ,   _min_h{h}
    ,   _longest_line{0}
    ,   _line_count{0}
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
    if (w < _min_w) w = _min_w;
    if (h < _min_h) h = _min_h;
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
        auto *rchar = _rendered_characters[_cursor_position-1];
        _cursor->set_parent(rchar);
        _cursor->set_relative_position({rchar->get_advance(), 0});
    }
    else {
        _cursor->set_parent(_topleft);
        _cursor->set_relative_position({0, 0});
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
    int this_line_length = measure_line();
    if (this_line_length > _longest_line) _longest_line = this_line_length;
    set_size(_longest_line, _line_count*20);
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
    decrement_cursor_position();
    delete rchar;
}

void TextArea::newline()
{
    auto *nl = new RenderedCharacter(this, U'\n', _font);
    Object *root = _topleft;
    if (_cursor_position) root = _rendered_characters[_cursor_position-1];
    nl->set_relative_position<TextArea>({0, root->get_absolute_position().y - 20});
    _text.insert(_text.begin() + _cursor_position, '\n');
    _rendered_characters.insert(_rendered_characters.begin()+_cursor_position, nl);

    increment_cursor_position();

    _line_count++;
    set_size(_longest_line, _line_count*20);
}

int TextArea::measure_line()
{
    if (_rendered_characters.empty()) return 0;
    int i = _cursor_position ? _cursor_position-1 : 0;
    RenderedCharacter *curr = _rendered_characters[i], *start = curr, *end = curr;
    while (true) {
        if ((start->get_char() == U'\n') || (start->get_parent() == _topleft)) {
            break;
        }
        start = (RenderedCharacter *)start->get_parent();
    }
    while (true) {
        const auto &children = end->get_children();
        if (children.empty()) break;
        Object *child = children.front();
        if (end->get_char() == '\n') {
            break;
        }
        end = (RenderedCharacter *)child;
    }

    return end->get_position().x - start->get_position().x;
}