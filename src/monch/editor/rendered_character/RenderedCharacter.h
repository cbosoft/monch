//
// Created by Christopher Boyle on 13/10/2021.
//

#ifndef MONCH_RENDEREDCHARACTER_H
#define MONCH_RENDEREDCHARACTER_H

#include <monch/rendering/vertex_buffer/VertexBuffer.h>
#include <monch/rendering/renderable/Renderable.h>
#include <monch/rendering/renderer/Renderer.h>
#include <monch/rendering/font/manager/FontManager.h>

class RenderedCharacter final: public Renderable {
public:

    RenderedCharacter(Object *parent, char32_t ch, Font *font);

    ~RenderedCharacter() noexcept override;

    void render_me() final;

    [[nodiscard]] int get_advance() const;

private:

    void update_vbuff_points();

    VertexBuffer *_vbuff;
    uint _texture_id;

    int _w, _h, _advance, _bearing_x, _bearing_y;

    Font *_font;
    char32_t _char;
};


#endif //MONCH_RENDEREDCHARACTER_H
