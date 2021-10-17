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
    virtual bool dictates_object_size_position() const { return false; }
    virtual WindowPoint get_size_of(const Object *object) { return object->get_size(); /* won't be called */ }
    virtual WindowPoint get_position_of(const Object *object) { return object->get_relative_position(this); /* won't be called */ }

    [[nodiscard]] NormalisedPoint get_norm(const WindowPoint &pt) const;

private:
    float _r, _g, _b;
    Quad _quad;
    uint _fbuff, _txtr;
};


#endif //MONCH_CONTAINER_H
