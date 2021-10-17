//
// Created by Christopher Boyle on 14/10/2021.
//

#ifndef MONCH_CONTAINER_H
#define MONCH_CONTAINER_H

#include <monch/rendering/renderable/Renderable.h>
#include <monch/rendering/quad/Quad.h>

class Container: public Renderable {
    MONCH_OBJECT("Container")
public:
    Container(Object *parent, int w, int h);

    void render() override;
    void after_children_rendered() final;
    void set_colour(float r, float g, float b);

    [[nodiscard]] NormalisedPoint get_norm(const WindowPoint &pt) const;

private:
    float _r, _g, _b;
    Quad _quad;
    uint _fbuff, _txtr;
};


#endif //MONCH_CONTAINER_H
