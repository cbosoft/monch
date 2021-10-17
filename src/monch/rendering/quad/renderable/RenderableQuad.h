//
// Created by Christopher Boyle on 17/10/2021.
//

#ifndef MONCH_RENDERABLEQUAD_H
#define MONCH_RENDERABLEQUAD_H

#include <monch/rendering/renderable/Renderable.h>
#include <monch/rendering/quad/Quad.h>

class RenderableQuad: public Renderable, public Quad {
public:
    RenderableQuad(Object *parent);

    void render() override;
};


#endif //MONCH_RENDERABLEQUAD_H
