//
// Created by Christopher Boyle on 12/10/2021.
//
#include <iostream>

#include "monch/rendering/gl.h"
#include "monch/rendering/renderer/Renderer.h"
#include "Window.h"


Window::Window()
    :   _should_quit(false)
    ,   _has_resized(false)
{
    Renderer::ref().get_window_size(_width, _height);
}


Window::~Window()
{
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}


Window &Window::ref()
{
    static Window window;
    return window;
}


void Window::run()
{
    // TODO launch event thread
    while (!_should_quit) {
        process_one_event();
        render();
    }
}


void Window::render_me()
{
    // Clear color buffer to black
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
}


void Window::after_children_rendered()
{
    Renderer::ref().swap_and_poll();
    _has_resized = false;
}


void Window::key_pressed(int key, int mods)
{
    if (key == GLFW_KEY_ESCAPE) {
        _should_quit = true;
    }
}

void Window::key_released(int key, int mods)
{
    (void) key;
    (void) mods;
    (void) this;
    // TODO?
}

void Window::character_input(char32_t ch)
{
    (void)this;
    unicode_char_to_cstr conv = {0};
    conv.unicode_codepoint = ch;
    std::cerr << conv.cstr;
}

int Window::get_width() const
{
    return _width;
}

int Window::get_height() const
{
    return _height;
}

bool Window::has_resized() const
{
    return _has_resized;
}

void Window::resized(int width, int height)
{
    _width = width;
    _height = height;
    _has_resized = true;
    render();
}