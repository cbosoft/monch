//
// Created by Christopher Boyle on 12/10/2021.
//
#include <iostream>

#include "monch/rendering/gl.h"
#include "monch/rendering/renderer/Renderer.h"
#include "EditorApp.h"


EditorApp::EditorApp()
    :   Container(nullptr, 1, 1)
    ,   _should_quit{false}
    ,   _has_resized{false}
    ,   _event_thread{nullptr}
    ,   _text_area{nullptr}
{
    add_type<EditorApp>();
    int w, h;
    Renderer::ref().get_window_size(w, h);
    set_size(w, h);
    _text_area = new TextArea(this, w, h);
    _text_area->set_colour(0.1, 0.1, 0.1);
    _text_area->set_position({0, 0});
}


EditorApp::~EditorApp()
{
    if (_event_thread) {
        _event_thread->join();
        delete _event_thread;
    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}


EditorApp &EditorApp::ref()
{
    static EditorApp window;
    return window;
}


void EditorApp::event_thread_loop()
{
    const double framerate = 60.;
    const uint update_period_ms = uint(1e3/framerate);
    while (!_should_quit)
    {
        this->process_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(update_period_ms));
    }
}


void EditorApp::run()
{
    const double framerate = 30.;
    const uint update_period_ms = uint(1e3/framerate);
    //_event_thread = new std::thread([this](){this->event_thread_loop();});
    auto &renderer = Renderer::ref();
    while (!_should_quit) {
        renderer.render(this);
        this->process_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(update_period_ms));
    }
}


void EditorApp::post_render()
{
    _has_resized = false;
}


void EditorApp::key_pressed(int key, int mods)
{
    (void) mods;
    switch (key) {
        case GLFW_KEY_ESCAPE:
            _should_quit = true;
            break;

        case GLFW_KEY_ENTER:
            _text_area->newline();
            break;

        case GLFW_KEY_BACKSPACE:
            _text_area->backspace();
            break;

        case GLFW_KEY_LEFT:
            _text_area->decrement_cursor_position();
            break;

        case GLFW_KEY_RIGHT:
            _text_area->increment_cursor_position();
            break;

        default:
            break;
    }
}

void EditorApp::key_released(int key, int mods)
{
    (void) key;
    (void) mods;
    (void) this;
    // TODO?
}

void EditorApp::character_input(char32_t ch)
{
    (void)this;
    unicode_char_to_cstr conv = {0};
    conv.unicode_codepoint = ch;
    std::cerr << conv.cstr;
    _text_area->add_char(ch);
}

bool EditorApp::has_resized() const
{
    return _has_resized;
}

void EditorApp::resized(int width, int height)
{
    set_size(width, height);
    _text_area->set_size(width, height);
    _has_resized = true;
    // render();
}