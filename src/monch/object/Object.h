//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_OBJECT_H
#define MONCH_OBJECT_H

#include <list>
#include <mutex>

#include <monch/util.h>
#include "event/Event.h"

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
    void process_one_event();

    void give_event(Event *event);

    void set_parent(Object *object);
    void insert_parent(Object *parent);
    void add_child(Object *object);
    void remove_child(Object *object);

    ConstObjectIter begin() const;
    ConstObjectIter end() const;

    [[nodiscard]] WindowPoint get_position() const;
    void set_position(const WindowPoint &pos);
    [[nodiscard]] WindowPoint get_relative_position() const;
    void set_relative_position(const WindowPoint &rel_pos);
    void increment_position(const WindowPoint &delta);

    [[nodiscard]] bool has_changed_position() const;

    bool is_a(const std::string &type);

protected:
    void set_not_moved();
    void add_type(const std::string &type_key);

private:
    bool is_a(std::size_t hsh);
    void add_type(std::size_t hsh);

    Event *get_next_event();
    std::mutex _m;
    std::list<Event *> _events;
    std::atomic_bool _should_stop_event_loop, _is_event_loop_running;
    std::atomic<WindowPoint> _rel_pos;
    std::atomic_bool _has_changed_position;

    Object *_parent;
    std::list<Object *> _children;
    std::list<std::size_t> _types;
};


#endif //MONCH_OBJECT_H
