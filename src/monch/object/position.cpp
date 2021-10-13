//
// Created by Christopher Boyle on 13/10/2021.
//

#include "Object.h"

WindowPoint Object::get_position() const
{
    WindowPoint rv = _rel_pos;
    if (_parent)
        rv += _parent->get_position();
    return rv;
}

void Object::set_position(const WindowPoint &pos)
{
    if (_parent) {
        _rel_pos = pos - _parent->get_position();
    }
    else {
        _rel_pos = pos;
    }
    _has_changed_position = true;
}

WindowPoint Object::get_relative_position() const
{
    return _rel_pos;
}

void Object::set_relative_position(const WindowPoint &rel_pos)
{
    _rel_pos = rel_pos;
    _has_changed_position = true;
}

void Object::increment_position(const WindowPoint &delta)
{
    _rel_pos = _rel_pos.load() + delta;
    _has_changed_position = true;
}