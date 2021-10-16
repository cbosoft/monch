//
// Created by Christopher Boyle on 12/10/2021.
//

#include <thread>
#include <monch/rendering/container/Container.h>
#include "Object.h"

static uint idcounter = 0;


Object::Object(Object *parent)
    :   _id{idcounter++}
    ,   _parent{nullptr}
    ,   _container{nullptr}
    ,   _has_invalid_position_scale{true}
    ,   _rel_pos({0, 0})
{
    add_type<Object>();
    set_parent(parent);
}


Object::~Object()
{
    // re
    if (_parent) {
        _parent->remove_child(this);
    }
    auto children = _children;
    for (auto *child : children)
        delete child;
}


Object *Object::get_parent() const
{
    return _parent;
}


bool Object::has_parent() const
{
    return _parent != nullptr;
}


bool Object::has_container() const
{
    return _container != nullptr;
}


Container *Object::get_container() const
{
    return _container;
}


void Object::set_parent(Object *object)
{
    if (_parent) {
        _parent->remove_child(this);
    }
    if (object == this) throw std::runtime_error("object cannot be parent of itself");
    _parent = object;
    _container = find_in_parents<Container>();
    if (object != nullptr)
        object->add_child(this);
    invalidate_position_scale();
}

void Object::insert_parent(Object *new_parent)
{
    if (new_parent) {
        new_parent->set_parent(_parent);
    }
    set_parent(new_parent);
    invalidate_position_scale();
}

void Object::add_child(Object *object)
{
    if (object == this) throw std::runtime_error("object cannot be child of itself");
    _children.push_back(object);
    auto existing_child_it = std::find(_children.begin(), _children.end(), object);
    if (existing_child_it == _children.end()) {
        _children.push_back(object);
    }
}

void Object::remove_child(Object *object)
{
    auto existing_child_it = std::find(_children.begin(), _children.end(), object);
    if (existing_child_it != _children.end()) {
        _children.remove(object);
    }
    else {
        // can't remove object; it is not child of this!
    }
}

ConstObjectIter Object::begin() const
{
    return _children.begin();
}

ConstObjectIter Object::end() const
{
    return _children.end();
}


void Object::add_type(std::size_t hsh)
{
    _types.push_back(hsh);
}


bool Object::is_a(std::size_t hsh)
{
    return std::any_of(_types.begin(), _types.end(), [hsh](std::size_t ohsh) -> bool {return ohsh==hsh;});
}


Object *Object::_find_in_children(std::size_t type_hsh)
{
    for (auto *child : *this) {
        if (child->is_a(type_hsh)) {
            return child;
        }
    }
    // TODO recursively find in children?
    return nullptr;
}


Object *Object::_find_in_parents(std::size_t type_hsh)
{
    if (!_parent) return nullptr;

    if (_parent->is_a(type_hsh)) return _parent;

    return _parent->_find_in_parents(type_hsh);
}