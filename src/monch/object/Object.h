//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_OBJECT_H
#define MONCH_OBJECT_H

#include <list>
#include <mutex>
#include <sstream>

#include <monch/util.h>
#include "event/Event.h"

class Object;
typedef std::list<Object *>::iterator ObjectIter;
typedef std::list<Object *>::const_iterator ConstObjectIter;

#define MONCH_OBJECT_BASE(TYPENAME) \
public:\
static std::size_t type_hash() \
{\
    static std::size_t hsh = std::hash<std::string>{}(TYPENAME);\
    return hsh; \
}\
[[nodiscard]] virtual std::string get_id() const\
{\
    std::stringstream ss;\
    ss << (TYPENAME) << "(" << _id << ")";\
    return ss.str();\
}

#define MONCH_OBJECT(TYPENAME) \
public:\
static std::size_t type_hash() \
{\
    static std::size_t hsh = std::hash<std::string>{}(TYPENAME);\
    return hsh; \
}\
[[nodiscard]] std::string get_id() const override\
{\
    std::stringstream ss;\
    ss << (TYPENAME) << "(" << _id << ")";\
    return ss.str();\
}


// generic object; reciever of events and such
class Object {
    MONCH_OBJECT_BASE("Object")
public:
    explicit Object(Object *parent=nullptr);
    virtual ~Object();
    Object(const Object &) =delete;
    Object &operator=(const Object &) =delete;

    void process_events();
    void process_one_event();

    void give_event(Event *event);

    [[nodiscard]] bool has_parent() const;
    [[nodiscard]] Object *get_parent() const;
    void set_parent(Object *object);
    void insert_parent(Object *parent);
    void add_child(Object *object);
    void remove_child(Object *object);

    template<typename T>
    T *find_in_children()
    {
        return (T *)_find_in_children(T::type_hash());
    }

    template<typename T>
    T *find_in_parents()
    {
        return (T *)_find_in_parents(T::type_hash());
    }

    template<typename T>
    bool is_a()
    {
        return is_a(T::type_hash());
    }

    ConstObjectIter begin() const;
    ConstObjectIter end() const;

    [[nodiscard]] WindowPoint get_position() const;
    void set_position(const WindowPoint &pos);
    [[nodiscard]] WindowPoint get_relative_position() const;
    void set_relative_position(const WindowPoint &rel_pos);
    void increment_position(const WindowPoint &delta);

    template<typename T>
    void set_relative_position(const WindowPoint &rel_pos)
    {
        set_relative_position(rel_pos, T::type_hash());
    }

    template<typename T>
    [[nodiscard]] WindowPoint get_relative_position() const
    {
        return get_relative_position(T::type_hash());
    }

    [[nodiscard]] bool has_changed_position() const;
    void set_not_moved();

protected:

    template<typename T>
    void add_type()
    {
        add_type(T::type_hash());
    }

    uint _id;

private:
    void add_type(std::size_t hsh);
    bool is_a(std::size_t hsh);
    Object *_find_in_children(std::size_t hash);
    Object *_find_in_parents(std::size_t hash);
    [[nodiscard]] WindowPoint get_relative_position(std::size_t relative_to_hsh) const;
    void set_relative_position(const WindowPoint &rel_pos, std::size_t relative_to_hsh);

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
