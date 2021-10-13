//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_UTIL_H
#define MONCH_UTIL_H

typedef unsigned int uint;

struct WindowPoint {
    int x, y;

    WindowPoint operator+(const WindowPoint &o) const
    {
        return {this->x+o.x, this->y+o.y};
    }

    WindowPoint &operator+=(const WindowPoint &o)
    {
        this->x += o.x;
        this->y += o.y;
        return *this;
    }

    WindowPoint operator-(const WindowPoint &o) const
    {
        return {this->x-o.x, this->y-o.y};
    }

    WindowPoint &operator-=(const WindowPoint &o)
    {
        this->x -= o.x;
        this->y -= o.y;
        return *this;
    }
};

struct NormalisedPoint {
    float x, y;

    NormalisedPoint operator+(const NormalisedPoint &o) const
    {
        return {this->x+o.x, this->y+o.y};
    }

    NormalisedPoint &operator+=(const NormalisedPoint &o)
    {
        this->x += o.x;
        this->y += o.y;
        return *this;
    }
    NormalisedPoint operator-(const NormalisedPoint &o) const
    {
        return {this->x-o.x, this->y-o.y};
    }

    NormalisedPoint &operator-=(const NormalisedPoint &o)
    {
        this->x -= o.x;
        this->y -= o.y;
        return *this;
    }
};

#endif //MONCH_UTIL_H
