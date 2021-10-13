#include <iostream>
#include <cmath>

#include "monch/rendering/vertex_buffer/VertexBuffer.h"
#include "monch/rendering/gl.h"
#include "monch/rendering/renderer/Renderer.h"
#include <monch/editor/app/EditorApp.h>
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

    RenderedCharacter(Object *parent, Character *c)
        :   Renderable(parent)
        ,   _texture_id{c->texture_id}
        ,   _w{c->w}
        ,   _h{c->h}
    {
        _vbuff = new VertexBuffer({ {0, 0}, {_w, 0}, {_w, _h}, {0, _h}});
        _vbuff->set_tex_coords({{0, 1}, {1, 1}, {1, 0}, {0, 0}});
        Renderer::ref().assign_shader(this, "font");
        set_relative_position({c->advance, 0});
    }

    ~RenderedCharacter() override
    {
        delete _vbuff;
    }

    void render_me() final
    {
        Renderer::ref().use_assigned_shader(this);
        glBindTexture(GL_TEXTURE_2D, _texture_id);
        if (has_changed_position()) {
            update_vbuff_points();
        }
        _vbuff->draw();
    }

private:

    void update_vbuff_points()
    {
        auto pt = get_position();
        uint x= pt.x, y=pt.y;
        _vbuff->set_points({ {x, y}, {x + _w, y}, {x + _w, y + _h}, {x, y + _h}});
    }

    VertexBuffer *_vbuff;
    uint _texture_id;

    uint _w, _h;

};


class MoveEvent final: public Event {
public:

    MoveEvent() : t{0} {}

    bool run(Object *owner) final
    {
        uint x = uint(std::sin(t)*100) + 100; t += 0.01;
        owner->set_position({x, 240});
        return false;
    }

    double t;
};


class BlockOfText final: public Renderable {
public:
    BlockOfText(Renderable *parent, const std::string &text)
        : Renderable(parent)
    {
        Font *font = FontManager::ref().get_font("iosevka-regular.ttf", 50);
        Object *anchor = this;
        for (char ch : text) {
            anchor = new RenderedCharacter(anchor, font->get_char(ch));
        }
    }

    void render_me() final
    {
        // nothing
    }
};

int main()
{
    EditorApp &win = EditorApp::ref();
    auto *sq = new BlockOfText(&win, "edo, edit => to eat");
    sq->set_position({20, 200});
    sq->give_event(new MoveEvent);
    (void) sq;
    win.run();
}
