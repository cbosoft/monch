//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_QUAD_H
#define MONCH_QUAD_H

#include <array>
#include <vector>

#include <monch/util.h>
#include <monch/rendering/Vertex.h>

class Renderable;
class Quad {
public:

    explicit Quad(Renderable *owner);
    ~Quad();

    void draw();

    void set_colour(float r, float g, float b, float a=1.);
    void set_tex_coords(const std::array<NormalisedPoint, 4> &points);
    void set_offset(const WindowPoint &off);
    void rescale();

private:

    void init();
    void sync_gl();

    uint _id, _va;
    std::array<Vertex, 4> _vertices;
    Renderable *_owner;
    std::atomic_bool _is_init, _sync_needed;
    WindowPoint _offset;

};


#endif //MONCH_QUAD_H
