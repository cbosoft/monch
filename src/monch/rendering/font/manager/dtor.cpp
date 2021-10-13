#include "FontManager.h"


FontManager::~FontManager()
{
    for(const auto &kv : this->font_cache) {
        delete kv.second;
    }

    this->font_cache.clear();

    FT_Done_FreeType(this->_ft_lib);
}
