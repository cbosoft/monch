#include "FontManager.h"


FontManager& FontManager::ref()
{
    static FontManager fm;
    return fm;
}
