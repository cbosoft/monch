//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_QUAD_H
#define MONCH_QUAD_H

#include <monch/rendering/renderable/Renderable.h>
#include <monch/rendering/vertex_buffer/VertexBuffer.h>

class Quad: public Renderable {

    MONCH_OBJECT("Quad")
public:
    Quad(Object *parent, int x, int y, int w, int h);

    void render() override;

    void set_colour(float r, float g, float b);

    int x, y, w, h;

private:
    VertexBuffer _vbuff;
};


#endif //MONCH_QUAD_H
