//
// Created by Christopher Boyle on 12/10/2021.
//

#include "Object.h"
#include <thread>

static uint idcounter = 0;


Object::Object(Object *parent)
    :   _id{idcounter++}
    ,   _is_event_loop_running{false}
    ,   _should_stop_event_loop{false}
    ,   _parent{parent}
    ,   _has_invalid_position_scale{true}
    ,   _rel_pos({0, 0})
{
    add_type<Object>();
    set_parent(parent);
}


Object::~Object()
{
    if (_parent) {
        _parent->remove_child(this);
    }
}


void Object::process_one_event()
{
    Event *event = get_next_event();

    if (event != nullptr) {

        if (event->should_run()) {

            // Run event. Is it finished?
            if (event->run(this)) {
                // finished, safe to delete it
                delete event;
            }
            else {
                // not finished, need to process it again later
                give_event(event);
            }
        } else {
            give_event(event);
        }
    }
}


void Object::process_events()
{
    for (uint i = 0; i < _events.size(); i++)
        process_one_event();


    for (Object *child : *this) {
        child->process_events();
    }
}


Event *Object::get_next_event()
{
    std::lock_guard<std::mutex> _l(_m);
    Event *event = nullptr;
    if (!_events.empty()) {
        event = _events.front();
        _events.pop_front();
    }
    return event;
}


void Object::give_event(Event *event)
{
    if (!event) return;
    std::lock_guard<std::mutex> _l(_m);
    _events.push_back(event);
}


Object *Object::get_parent() const
{
    return _parent;
}


bool Object::has_parent() const
{
    return _parent != nullptr;
}


void Object::set_parent(Object *object)
{
    if (_parent) {
        _parent->remove_child(this);
    }
    if (object == this) throw std::runtime_error("object cannot be parent of itself");
    _parent = object;
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
    if (existing_child_it == _children.end()) {
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