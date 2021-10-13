//
// Created by Christopher Boyle on 12/10/2021.
//
#include <iostream>

#include "monch/rendering/gl.h"
#include "monch/rendering/renderer/Renderer.h"
#include "EditorApp.h"


EditorApp::EditorApp()
    :   _should_quit{false}
    ,   _has_resized{false}
    ,   _width{0}
    ,   _height{0}
    ,   _event_thread{nullptr}
    ,   _text_area{nullptr}
{
    Renderer::ref().get_window_size(_width, _height);
    _text_area = new TextArea(this);
    _text_area->set_position({10, 200});
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
    _event_thread = new std::thread([this](){this->event_thread_loop();});
    while (!_should_quit) {
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(update_period_ms));
    }
}


void EditorApp::render_me()
{
    // Clear color buffer to black
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
}


void EditorApp::after_children_rendered()
{
    Renderer::ref().swap_and_poll();
    _has_resized = false;
}


void EditorApp::key_pressed(int key, int mods)
{
    if (key == GLFW_KEY_ESCAPE) {
        _should_quit = true;
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

int EditorApp::get_width() const
{
    return _width;
}

int EditorApp::get_height() const
{
    return _height;
}

bool EditorApp::has_resized() const
{
    return _has_resized;
}

void EditorApp::resized(int width, int height)
{
    _width = width;
    _height = height;
    _has_resized = true;
    render();
}