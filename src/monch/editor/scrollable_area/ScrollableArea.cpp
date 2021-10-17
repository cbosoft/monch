//
// Created by Christopher Boyle on 17/10/2021.
//

#include "ScrollableArea.h"

ScrollableArea::ScrollableArea(Object *parent, Object *payload)
    :   Container(parent, 10, 10)
    ,   _payload{payload}
    ,   _vel_x{0}
    ,   _vel_y{0}
    ,   _friction{0.5}
{
    _payload->set_parent(this);
}

void ScrollableArea::scroll(int dx, int dy)
{
    _payload->increment_position({dx, dy});
}

void ScrollableArea::set_scroll_velocity(double x, double y)
{
    _vel_x = x;
    _vel_y = y;
}

void ScrollableArea::render()
{
    timestep_scrolling();
    //WindowPoint p = _payload->get_relative_position();
    //WindowPoint ps = _payload->get_size(), s = get_size();
    //if (p.x - ps.x < s.x) p.x = (p.x*1)/10;
    //if (p.y - ps.y < s.y) p.y = (p.y*1)/10;
    //if (p.x < 0) p.x = (p.x*1)/10; // don't go past left wall
    //if (p.y < 0) p.y = (p.y*1)/10; // don't go past floor
    //_payload->set_relative_position(p);
    Container::render();
}

void ScrollableArea::timestep_scrolling()
{
    int dx = 0, dy = 0;
    if (_vel_x > 0.1 || _vel_x < -0.1) {
        dx = int(_vel_x);
        _vel_x *= (1.0 - _friction);
    }

    if (_vel_y > 0.1 || _vel_y < -0.1) {
        dy = int(_vel_y);
        _vel_y *= (1.0 - _friction);
    }

    if (dx > 10) dx = 10;
    if (dx < -10) dx = -10;
    if (dy > 10) dy = 10;
    if (dy < -10) dy = -10;

    if (dx || dy) scroll(dx, dy);
}