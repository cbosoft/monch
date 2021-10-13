//
// Created by Christopher Boyle on 13/10/2021.
//

#ifndef MONCH_TEXTAREA_H
#define MONCH_TEXTAREA_H

#include <vector>
#include <map>
#include <monch/rendering/renderable/Renderable.h>

class Font;
class TextArea final: public Renderable {
public:
    explicit TextArea(Object *parent);

    void add_char(char32_t c);

    void render_me() final;

private:
    std::vector<char32_t> _text;
    std::vector<Renderable *> _rendered_characters;
    int _cursor_position;
    Font *_font;
};


#endif //MONCH_TEXTAREA_H
