//
// Created by Christopher Boyle on 16/10/2021.
//

#include "Object.h"

void Object::invalidate_position_scale()
{
    _has_invalid_position_scale = true;
}


bool Object::has_invalid_position_scale() const
{
    if (_has_invalid_position_scale)
        return true;

    if (_parent && _parent->has_invalid_position_scale())
        return true;

    return false;
}


void Object::validate_position_scale()
{
    _has_invalid_position_scale = false;
}