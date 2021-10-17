//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_OBJECT_H
#define MONCH_OBJECT_H

#include <list>
#include <mutex>
#include <sstream>

#include <monch/util.h>
#include "monch/events/event/Event.h"

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

class Container;
class Object {
    MONCH_OBJECT_BASE("Object")
public:
    explicit Object(Object *parent=nullptr);
    virtual ~Object();
    Object(const Object &) =delete;
    Object &operator=(const Object &) =delete;

    [[nodiscard]] bool has_parent() const;
    [[nodiscard]] Object *get_parent() const;
    [[nodiscard]] bool has_container() const;
    [[nodiscard]] Container *get_container() const;
    void set_parent(Object *object);
    void insert_parent(Object *parent);
    void add_child(Object *object);
    void remove_child(Object *object);
    [[nodiscard]] int count_children() const;
    [[nodiscard]] const std::list<Object *> &get_children() const;

    Object *find_in_children(const Object *object) const;
    Object *find_in_parents(const Object *object) const;
    template<typename T> T *find_in_children() const { return (T *)_find_in_children(T::type_hash()); }
    template<typename T> T *find_in_parents() const { return (T *)_find_in_parents(T::type_hash()); }

    template<typename T> bool is_a() { return is_a(T::type_hash()); }

    ConstObjectIter begin() const;
    ConstObjectIter end() const;

    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
    [[nodiscard]] WindowPoint get_size() const;
    virtual void set_size(int width, int height);
    [[nodiscard]] WindowPoint get_absolute_position() const;
    void set_absolute_position(const WindowPoint &pos);
    [[nodiscard]] WindowPoint get_position() const;
    void set_position(const WindowPoint &pos);
    [[nodiscard]] WindowPoint get_relative_position() const;
    void set_relative_position(const WindowPoint &rel_pos);
    [[nodiscard]] WindowPoint get_relative_position(const Object *relto) const;
    void set_relative_position(const WindowPoint &rel_pos, const Object *relto);
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

    [[nodiscard]] bool has_invalid_position_scale() const;
    void validate_position_scale();
    void invalidate_position_scale();

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
    Object *_find_in_children(std::size_t hash) const;
    Object *_find_in_parents(std::size_t hash) const;
    [[nodiscard]] WindowPoint get_relative_position(std::size_t relative_to_hsh) const;
    void set_relative_position(const WindowPoint &rel_pos, std::size_t relative_to_hsh);

    std::atomic<WindowPoint> _rel_pos; // relative to parent
    std::atomic<WindowPoint> _size;
    std::atomic_bool _has_invalid_position_scale;

    Object *_parent;
    Container *_container;
    std::list<Object *> _children;
    std::list<std::size_t> _types;
};


#endif //MONCH_OBJECT_H
