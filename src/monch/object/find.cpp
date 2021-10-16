//
// Created by Christopher Boyle on 16/10/2021.
//

#include "Object.h"


Object *Object::_find_in_children(std::size_t type_hsh) const
{
    for (auto *child : *this) {
        if (child->is_a(type_hsh)) {
            return child;
        }
    }
    // TODO recursively find in children?
    return nullptr;
}


Object *Object::_find_in_parents(std::size_t type_hsh) const
{
    if (!_parent) return nullptr;

    if (_parent->is_a(type_hsh)) return _parent;

    return _parent->_find_in_parents(type_hsh);
}


Object *Object::find_in_children(const Object *object) const
{
    for (auto *child : *this) {
        if (child == object) {
            return child;
        }
    }
    // TODO recursively find in children?
    return nullptr;
}


Object *Object::find_in_parents(const Object *object) const
{
    if (!_parent) return nullptr;

    if (_parent == object) return _parent;

    return _parent->find_in_parents(object);
}