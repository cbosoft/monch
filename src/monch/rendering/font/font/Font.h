#pragma once
#include <map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <monch/util.h>

struct Character {
    uint texture_id;
    uint w;
    uint h;
    uint bearing_x;
    uint bearing_y;
    uint advance;
};


class Font {
public:
    Font(FT_Face face, int height);
    ~Font();

    Character *get_char(unsigned long c);

private:

    void load_char_from_font(unsigned long charcode);

    std::map<unsigned long, Character *> chars;
    FT_Face _face;
};