//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_QUAD_H
#define MONCH_QUAD_H

#include "monch/rendering/renderable/Renderable.h"

class Quad: public Renderable {
public:
    Quad(int x, int y, int w, int h);

    void render_me() override;

    int x, y, w, h;

};


#endif //MONCH_QUAD_H
