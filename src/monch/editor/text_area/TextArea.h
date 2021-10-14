//
// Created by Christopher Boyle on 13/10/2021.
//

#ifndef MONCH_TEXTAREA_H
#define MONCH_TEXTAREA_H

#include <vector>
#include <map>
#include <monch/rendering/renderable/Renderable.h>

class Font;
class RenderedCharacter;
class TextArea final: public Renderable {
public:
    explicit TextArea(Object *parent);

    void add_char(char32_t c);

    void render_me() final;

    void backspace();
    void newline();

    void increment_cursor_position();
    void decrement_cursor_position();

private:
    void reposition_cursor();
    std::vector<char32_t> _text;
    std::vector<RenderedCharacter *> _rendered_characters;
    int _cursor_position;
    Font *_font;
    Renderable *_cursor;
};


#endif //MONCH_TEXTAREA_H
