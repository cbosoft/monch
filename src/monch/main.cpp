#include <iostream>

#include "monch/rendering/vertex_buffer/VertexBuffer.h"
#include "monch/rendering/gl.h"
#include "monch/rendering/renderer/Renderer.h"
#include "monch/rendering/window/Window.h"
#include <monch/rendering/font/manager/FontManager.h>
#include <monch/rendering/font/font/Font.h>

class Square final: public Renderable {
public:

    Square(Object *parent, const std::vector<WindowPoint> &points)
        :   Renderable(parent)
    {
        _vbuff = new VertexBuffer(points);
        Renderer::ref().assign_shader(this, "default");
    }

    ~Square() override
    {
        delete _vbuff;
    }

    void render_me() final
    {
        Renderer::ref().use_assigned_shader(this);
        _vbuff->draw();
    }

private:

    VertexBuffer *_vbuff;

};


class RenderedCharacter final: public Renderable {
public:

    RenderedCharacter(Object *parent, Character *c, uint &x, uint &y)
        :   Renderable(parent)
        ,   _texture_id{c->texture_id}
    {
        _vbuff = new VertexBuffer({ {x, y}, {x + c->w, y}, {x + c->w, y + c->h}, {x, y + c->h}});
        _vbuff->set_tex_coords({{0, 1}, {1, 1}, {1, 0}, {0, 0}});
        Renderer::ref().assign_shader(this, "font");
        x += c->advance;
    }

    ~RenderedCharacter() override
    {
        delete _vbuff;
    }

    void render_me() final
    {
        Renderer::ref().use_assigned_shader(this);
        glBindTexture(GL_TEXTURE_2D, _texture_id);
        _vbuff->draw();
    }

private:

    VertexBuffer *_vbuff;
    uint _texture_id;

};


class BlockOfText final: public Renderable {
public:
    BlockOfText(Renderable *parent, const std::string &text)
        : Renderable(parent)
    {
        uint scrx = 0, scry = 300;
        Font *font = FontManager::ref().get_font("iosevka-regular.ttf", 100);
        for (char ch : text) {
            auto *rc = new RenderedCharacter(this, font->get_char(ch), scrx, scry);
            (void) rc;
        }
    }

    void render_me() final
    {
        // nothing
    }
};

int main()
{
    Window &win = Window::ref();
    auto *sq = new BlockOfText(&win, "foo");
    (void) sq;
    win.run();
}
