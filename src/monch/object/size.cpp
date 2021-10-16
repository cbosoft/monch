//
// Created by Christopher Boyle on 16/10/2021.
//

#include "Object.h"

WindowPoint Object::get_size() const
{
    return _size;
}


void Object::set_size(int width, int height)
{
    _size = WindowPoint{width, height};
    invalidate_position_scale();
}


int Object::get_width() const
{
    return _size.load().x;
}


int Object::get_height() const
{
    return _size.load().y;
}