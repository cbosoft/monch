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
    ,   _has_changes{false}
{
    add_type<EditorApp>();
    int w, h;
    Renderer::ref().get_window_size(w, h);
    Container::set_size(w, h); // explicitly invoke method of container as is virtual method.
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


void EditorApp::run()
{
    const double framerate = 30.;
    const uint update_period_ms = uint(1e3/framerate);
    _event_thread = new std::thread([this](){ this->run_event_loop();});
    auto &renderer = Renderer::ref();
    while (!_should_quit) {
        {
            std::lock_guard<std::mutex> _l(objects_mutex);
            renderer.render(this);
        }
        //run_through_events_once();
        std::this_thread::sleep_for(std::chrono::milliseconds(update_period_ms));
    }
    stop_event_loop();
}

void EditorApp::render()
{
    bool should_render = _has_changes;
    if (!should_render) {
        long millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _time_last_render).count();
        if (millis > 1000) {
            // if its been a while, force re-render
            should_render = true;
        }
    }

    if (should_render) {
        Container::render();
        _time_last_render = std::chrono::system_clock::now();
        _has_changes = false;
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
            _has_changes = true;
            break;

        case GLFW_KEY_BACKSPACE:
            _text_area->backspace();
            _has_changes = true;
            break;

        case GLFW_KEY_LEFT:
            _text_area->decrement_cursor_position();
            _has_changes = true;
            break;

        case GLFW_KEY_RIGHT:
            _text_area->increment_cursor_position();
            _has_changes = true;
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
    _text_area->add_char(ch);
    _has_changes = true;
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