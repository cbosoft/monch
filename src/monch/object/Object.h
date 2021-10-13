//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_OBJECT_H
#define MONCH_OBJECT_H

#include <list>
#include <mutex>

#include "monch/object/event/Event.h"

class Object;
typedef std::list<Object *>::iterator ObjectIter;
typedef std::list<Object *>::const_iterator ConstObjectIter;

// generic object; reciever of events and such
class Object {
public:
    explicit Object(Object *parent=nullptr);
    virtual ~Object();
    Object(const Object &) =delete;
    Object &operator=(const Object &) =delete;

    void process_events();
    bool process_one_event();

    void give_event(Event *event);
    void stop();

    void set_parent(Object *object);
    void add_child(Object *object);
    void remove_child(Object *object);

    ConstObjectIter begin() const;
    ConstObjectIter end() const;

private:
    Event *get_next_event();
    std::mutex _m;
    std::list<Event *> _events;
    std::atomic_bool _should_stop_event_loop, _is_event_loop_running;

    Object *_parent;
    std::list<Object *> _children;
};


#endif //MONCH_OBJECT_H
