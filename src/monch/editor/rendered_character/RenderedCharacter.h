//
// Created by Christopher Boyle on 13/10/2021.
//

#ifndef MONCH_RENDEREDCHARACTER_H
#define MONCH_RENDEREDCHARACTER_H

#include <monch/rendering/quad/Quad.h>
#include <monch/rendering/renderable/Renderable.h>
#include <monch/rendering/renderer/Renderer.h>
#include <monch/rendering/font/manager/FontManager.h>

class RenderedCharacter final: public Renderable {
    MONCH_OBJECT("RenderedCharacter")
public:

    RenderedCharacter(Object *parent, char32_t ch, Font *font);

    ~RenderedCharacter() noexcept override;

    void render() final;

    [[nodiscard]] int get_advance() const;

private:

    Quad *_quad;
    uint _texture_id;

    int _advance, _bearing_x, _bearing_y;

    Font *_font;
    char32_t _char;
};


#endif //MONCH_RENDEREDCHARACTER_H
