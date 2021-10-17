//
// Created by Christopher Boyle on 13/10/2021.
//

#ifndef MONCH_TEXTAREA_H
#define MONCH_TEXTAREA_H

#include <vector>
#include <map>
#include <monch/rendering/container/Container.h>
#include <monch/editor/app/EditorApp.h>

class Font;
class RenderedCharacter;
class TextArea final: public Container {
    MONCH_OBJECT("TextArea")
public:
    explicit TextArea(Object *parent, int w, int h);

    void add_char(char32_t c);

    void backspace();
    void newline();

    void increment_cursor_position();
    void decrement_cursor_position();

    class AddCharEvent : public Event {
    public:
        AddCharEvent(char32_t ch) : _ch{ch} {}

        bool run(Object *obj) override
        {
            auto *ta = (TextArea *)obj;
            ta->add_char(_ch);
            EditorApp::ref().set_has_changes();
            return true;
        }

    private:
        char32_t _ch;
    };

private:
    int measure_line();
    void set_size(int w, int h) final;
    void reposition_cursor();
    std::vector<char32_t> _text;
    std::vector<RenderedCharacter *> _rendered_characters;
    int _cursor_position;
    Font *_font;
    Renderable *_cursor;
    Object *_topleft;

    int _min_w, _min_h;
    int _longest_line, _line_count;
};


#endif //MONCH_TEXTAREA_H
