//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_RENDERABLE_H
#define MONCH_RENDERABLE_H

#include "monch/object/Object.h"

class Renderable: public Object {
public:

    explicit Renderable(Object *parent=nullptr);
    ~Renderable() override =default;

protected:
    virtual void render_me() =0;
    virtual void after_children_rendered();
    void render();

private:

    friend class Renderer;

};


#endif //MONCH_RENDERABLE_H
