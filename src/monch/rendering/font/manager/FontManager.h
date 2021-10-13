#pragma once

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <monch/rendering/font/font/Font.h>

class FontManager {
public:
    static FontManager &ref();
    ~FontManager();

    Font *get_font(const std::string &path, int size);

private:
    FontManager();

    Font *create_front(const std::string &path, int size);

    FT_Library _ft_lib;
    std::map<std::string, Font *> font_cache;
};
