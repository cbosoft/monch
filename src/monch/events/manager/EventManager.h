//
// Created by Christopher Boyle on 16/10/2021.
//

#ifndef MONCH_EVENTMANAGER_H
#define MONCH_EVENTMANAGER_H

#include <list>
#include <mutex>

class Event;
class Object;
struct EventAndObject {
    Event *event;
    Object *object;
};

class EventManager {
public:
    EventManager();
    ~EventManager();

    void run_event_loop();
    void run_through_events_once();
    void add_event(Event *event, Object *object);
    void stop_event_loop();

protected:
    long loop_delay_ms;

private:
    void process_one_event();
    EventAndObject *get_next_event();
    void add_event(EventAndObject *evobj);

    std::mutex _m;
    std::list<EventAndObject *> _events;
    std::atomic_bool _should_stop_event_loop;
};


#endif //MONCH_EVENTMANAGER_H
