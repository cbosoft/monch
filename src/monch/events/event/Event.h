//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_EVENT_H
#define MONCH_EVENT_H

class Object;
// abstract class for telling an Object to do something
class Event {
public:

    Event() =default;
    virtual ~Event() =default;

    virtual bool should_run() { return true; };
    virtual bool run(Object *object) =0;

};


#endif //MONCH_EVENT_H
