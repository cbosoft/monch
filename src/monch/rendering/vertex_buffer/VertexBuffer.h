//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_VERTEXBUFFER_H
#define MONCH_VERTEXBUFFER_H

#include <array>
#include <vector>

#include <monch/util.h>

struct Vertex {
    float x, y, z;
    float r, g, b, a;
    float s, t;
};

class VertexBuffer {
public:

    VertexBuffer(const std::vector<WindowPoint> &points);
    ~VertexBuffer();

    void draw();

    void set_points(const std::vector<WindowPoint> &points);
    void set_colour(float r, float g, float b, float a=1.);
    void set_tex_coords(const std::vector<NormalisedPoint> &points);
    void rescale();

private:

    void sync_gl() const;

    uint _id, _va;
    std::vector<WindowPoint> _unscaled;
    std::vector<Vertex> _vertices;

};


#endif //MONCH_VERTEXBUFFER_H
