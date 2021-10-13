#include <sstream>
#include <iostream>

#include "font_path.h"
#include "FontManager.h"


Font *FontManager::create_front(const std::string &name, int size)
{
    FT_Face face;
    std::string path = FONT_PATH"/" + name;
    if (FT_New_Face(this->_ft_lib, path.c_str(), 0, &face)) {
        std::cerr << path << " failed" << std::endl;
        path = FONT_FALLBACK_PATH"/" + name;
        if (FT_New_Face(this->_ft_lib, path.c_str(), 0, &face)) {
            std::cerr << path << " failed\n";
            std::cerr << "Could not load font \"" << name << "\"\n";
            return nullptr;
        }
    }

    return new Font(face, size);
}


Font *FontManager::get_font(const std::string &name, int font_size)
{
    std::stringstream ss;
    ss << name << "@" << font_size;
    std::string key = ss.str();

    auto it = this->font_cache.find(key);
    if (it != font_cache.end()) {
        return it->second;
    }

    Font *font = this->create_front(name, font_size);
    if (font) {
        this->font_cache[key] = font;
    }
    else if (!this->font_cache.empty()) {
        auto kv = this->font_cache.begin();
        std::cerr << "Falling back on \"" << kv->first << "\".";
        font = kv->second;
    }
    else {
        std::cerr << "Could not load requested font \"" << name << "\" and no fallback found.\n";
    }

    return font;
}