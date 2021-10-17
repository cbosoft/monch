//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_POLYGON_H
#define MONCH_POLYGON_H

#include <array>
#include <vector>

#include <monch/rendering/Vertex.h>
#include <monch/util.h>

class Renderable;
class Polygon {
public:

    explicit Polygon(Renderable *owner, uint n);
    ~Polygon();

    void draw();

    void set_points(const std::vector<WindowPoint> &points);
    void set_colour(float r, float g, float b, float a=1.);
    void set_tex_coords(const std::vector<NormalisedPoint> &points);
    void rescale();

private:

    void init();
    void sync_gl();

    uint _id, _va;
    std::vector<WindowPoint> _unscaled;
    std::vector<Vertex> _vertices;
    Renderable *_owner;
    std::atomic_bool _is_init, _sync_needed;

};


#endif //MONCH_POLYGON_H
