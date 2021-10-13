//
// Created by Christopher Boyle on 12/10/2021.
//

#include "Object.h"
#include <thread>


Object::Object(Object *parent)
    :   _is_event_loop_running{false}
    ,   _should_stop_event_loop{false}
    ,   _parent{parent}
{
    set_parent(parent);
}


Object::~Object()
{
    stop();
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

    for (Object *child : *this) {
        child->process_one_event();
    }
}


void Object::process_events()
{
    _is_event_loop_running = true;
    while (!_should_stop_event_loop) {
        process_one_event();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    _is_event_loop_running = false;
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


void Object::stop()
{
    _should_stop_event_loop = true;
    while (_is_event_loop_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


void Object::set_parent(Object *object)
{
    _parent = object;
    if (object != nullptr)
        object->add_child(this);
}

void Object::add_child(Object *object)
{
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