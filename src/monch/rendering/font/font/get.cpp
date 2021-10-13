#include "Font.h"


Character *Font::get_char(unsigned long c)
{
    auto it = this->chars.find(c);

    if (it == this->chars.end()) {
        this->load_char_from_font(c);
        return this->chars[c];
    }

    return it->second;
}