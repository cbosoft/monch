//
// Created by Christopher Boyle on 12/10/2021.
//
#include <iostream>

#include "monch/rendering/gl.h"
#include "monch/rendering/renderer/Renderer.h"
#include "Window.h"


Window::Window()
    :   _glfw_window{nullptr}
    ,   _should_quit(false)
    ,   _has_resized(false)
{
    init();
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

static void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) window;
    (void) scancode;
    switch (action) {
        case GLFW_PRESS:
            Window::ref().key_pressed(key, mods);
            break;
        case GLFW_RELEASE:
            Window::ref().key_released(key, mods);
            break;
        default:
        case GLFW_REPEAT:
            break;
    }
}

static void window_character_callback(GLFWwindow* window, unsigned int codepoint)
{
    (void) window;
    Window::ref().character_input(codepoint);
}


void window_size_callback(GLFWwindow *notused, int wid, int hgt)
{
    (void) notused;
    auto &win = Window::ref();
    win._width = wid;
    win._height = hgt;
    win._has_resized = true;
    win.render();
}


void Window::init()
{
    // Open a window and create its OpenGL context
    _glfw_window = Renderer::ref().init_window();

    // enable vsync
    glfwSwapInterval(1);

    // register input callbacks
    glfwSetKeyCallback(_glfw_window, &window_key_callback);
    glfwSetCharCallback(_glfw_window, &window_character_callback);

    glfwGetWindowSize(_glfw_window, &_width, &_height);
    glfwSetWindowSizeCallback(_glfw_window, &window_size_callback);
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
    glfwSwapBuffers(_glfw_window);
    glfwPollEvents();
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