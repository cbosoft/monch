//
// Created by Christopher Boyle on 12/10/2021.
//

#include <iostream>

#include "monch/rendering/gl.h"
#include "Renderer.h"
#include <monch/editor/app/EditorApp.h>
#include "shaders.h"

Renderer::Renderer()
    :   _glfw_window{nullptr}
{
    init();
}

Renderer &Renderer::ref()
{
    static Renderer renderer;
    return renderer;
}

void Renderer::init()
{
    // Initialise GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        exit(1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto *win = glfwCreateWindow(640, 480, "monch", nullptr, nullptr);
    if (!win) {
        std::cerr << "Failed to open window." << std::endl;
        exit(1);
    }

#ifndef MACOS
    // GLEW required on windows/linux
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("Failed to init GLEW.");
#endif

    float x_scale, y_scale;
    glfwGetWindowContentScale(win, &x_scale, &y_scale);
    std::cerr << x_scale << " " << y_scale << std::endl;

    glfwMakeContextCurrent(win);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    load_shader_program(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE, "default");
    load_shader_program(VERTEX_SHADER_SOURCE, FONT_SHADER_SOURCE, "font");
    load_shader_program(VERTEX_SHADER_SOURCE, TEXTURE_SHADER_SOURCE, "texture");

    error_check("Renderer::init() -> after load shaders");
    _glfw_window = win;

    // enable vsync
    glfwSwapInterval(1);

    // register input callbacks
    glfwSetKeyCallback(_glfw_window, &window_key_callback);
    glfwSetCharCallback(_glfw_window, &window_character_callback);

    glfwSetWindowSizeCallback(_glfw_window, &window_size_callback);
    error_check("Renderer::init() -> after set callbacks");
}


NormalisedPoint Renderer::convert_window_to_normal(const WindowPoint &pt)
{
    auto &win = EditorApp::ref();
    float x = float(pt.x)/float(win.get_width())*2.f - 1.f, y = float(pt.y)/float(win.get_height())*2.f - 1.f;
    return {x, y};
}


void Renderer::get_window_size(int &width, int &height) const
{
    glfwGetWindowSize(_glfw_window, &width, &height);
}


void Renderer::render(Object *root)
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    _recursive_render(root);
    glfwSwapBuffers(_glfw_window);
    glfwPollEvents();
}

void Renderer::_recursive_render(Object *obj)
{
    if (obj->is_a<Renderable>()) {
        ((Renderable *)obj)->render();
    }

    for (auto *child : *obj) {
        _recursive_render(child);
    }

    if (obj->is_a<Renderable>()) {
        auto *rbl = (Renderable *)obj;
        rbl->after_children_rendered();
        rbl->post_render();
    }

    obj->set_not_moved();
}