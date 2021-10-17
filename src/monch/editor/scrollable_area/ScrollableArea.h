//
// Created by Christopher Boyle on 17/10/2021.
//

#ifndef MONCH_SCROLLABLEAREA_H
#define MONCH_SCROLLABLEAREA_H

#include <monch/rendering/container/Container.h>

class ScrollableArea: public Container {
public:
    ScrollableArea(Object *parent, Object *payload);

    void scroll(int dx, int dy);
    void set_scroll_velocity(double x, double y);

    void render() override;

private:
    void timestep_scrolling();
    double _vel_x, _vel_y, _friction;
    Object *_payload;
};


#endif //MONCH_SCROLLABLEAREA_H
