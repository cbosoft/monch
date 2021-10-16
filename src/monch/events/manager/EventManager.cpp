//
// Created by Christopher Boyle on 16/10/2021.
//
#include <thread>
#include <chrono>
#include <iostream>

#include <monch/util.h>
#include "../event/Event.h"
#include "EventManager.h"

EventManager::EventManager()
    : loop_delay_ms{100}
    , _should_stop_event_loop{false}
{

}

EventManager::~EventManager()
{
    for (auto *evobj : _events ) {
        delete evobj->event;
        delete evobj;
        // object is owned+deleted elsewhere
    }
}



void EventManager::process_one_event()
{
    EventAndObject *evobj = get_next_event();
    Event *event = evobj->event;
    Object *obj = evobj->object;

    if (event != nullptr) {

        if (event->should_run() ) {
            std::lock_guard<std::mutex> _l(objects_mutex);
            // Run event. Is it finished?
            if (event->run(obj)) {
                // finished, safe to delete it
                delete event;
            }
            else {
                // not finished, need to process it again later
                add_event(evobj);
            }
        }
        else {
            add_event(evobj);
        }
    }
}


void EventManager::run_through_events_once()
{
    for (uint i = 0; i < _events.size(); i++)
        process_one_event();
}


EventAndObject *EventManager::get_next_event()
{
    std::lock_guard<std::mutex> _l(_m);
    EventAndObject *evobj = nullptr;
    if (!_events.empty()) {
        evobj = _events.front();
        _events.pop_front();
    }
    return evobj;
}


void EventManager::add_event(Event *event, Object *obj)
{
    if (!event) return;
    add_event(new EventAndObject{event, obj});
}

void EventManager::add_event(EventAndObject *evobj)
{
    std::lock_guard<std::mutex> _l(_m);
    _events.push_back(evobj);
}

void EventManager::stop_event_loop()
{
    _should_stop_event_loop = true;
}

void EventManager::run_event_loop()
{
    _should_stop_event_loop = false;
    while (!_should_stop_event_loop) {
        run_through_events_once();
        std::this_thread::sleep_for(std::chrono::milliseconds(loop_delay_ms));
    }
}