//
// Created by Christopher Boyle on 13/10/2021.
//

#include <monch/rendering/container/Container.h>
#include "Object.h"

WindowPoint Object::get_absolute_position() const
{
    WindowPoint rv = _rel_pos;
    if (_parent)
        rv += _parent->get_absolute_position();
    return rv;
}

void Object::set_absolute_position(const WindowPoint &pos)
{
    if (_parent) {
        _rel_pos = pos - _parent->get_absolute_position();
    }
    else {
        _rel_pos = pos;
    }
    invalidate_position_scale();
}

WindowPoint Object::get_relative_position() const
{
    return _rel_pos;
}

WindowPoint Object::get_relative_position(std::size_t relative_to_hsh) const
{
    if (!_parent) {
        throw std::runtime_error("can't get relative pos; no parent of that type found");
    }

    if (_parent->is_a(relative_to_hsh)) {
        return _rel_pos;
    }
    else {
        return WindowPoint(_rel_pos) + _parent->get_relative_position(relative_to_hsh);
    }
}

WindowPoint Object::get_relative_position(const Object *other) const
{
    if (!_parent) {
        throw std::runtime_error("can't get relative pos; object not found");
    }

    if (_parent == other) {
        return _rel_pos;
    }
    else {
        return WindowPoint(_rel_pos) + _parent->get_relative_position(other);
    }
}

void Object::set_relative_position(const WindowPoint &rel_pos)
{
    _rel_pos = rel_pos;
    invalidate_position_scale();
}

void Object::set_relative_position(const WindowPoint &rel_pos, std::size_t relative_to_hsh)
{
    WindowPoint diff = get_relative_position(relative_to_hsh);
    _rel_pos = rel_pos - diff;
}

void Object::set_relative_position(const WindowPoint &rel_pos, const Object *other)
{
    WindowPoint diff = get_relative_position(other);
    _rel_pos = rel_pos - diff;
}

void Object::increment_position(const WindowPoint &delta)
{
    _rel_pos = _rel_pos.load() + delta;
    invalidate_position_scale();
}


WindowPoint Object::get_position() const
{
    if (has_container()) {
        if (_container->dictates_object_size_position()) {
            return _container->get_position_of(this);
        }
        else {
            return get_relative_position(_container);
        }
    }
    else {
        return get_absolute_position();
    }
}


void Object::set_position(const WindowPoint &pt)
{
    if (has_container()) {
        set_relative_position(pt, _container); // TODO not working properly?
    }
    else {
        set_absolute_position(pt);
    }
}