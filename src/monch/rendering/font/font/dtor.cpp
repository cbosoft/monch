#include "Font.h"

Font::~Font()
{
    FT_Done_Face(this->_face);
}