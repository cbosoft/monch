//
// Created by Christopher Boyle on 12/10/2021.
//

#include <iostream>

#include "monch/rendering/gl.h"
#include "Renderer.h"
#include <monch/rendering/window/Window.h>
#include "shaders.h"

Renderer &Renderer::ref()
{
    static Renderer renderer;
    return renderer;
}

GLFWwindow *Renderer::init_window()
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

    glfwMakeContextCurrent(win);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    load_shader_program(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE, "default");
    load_shader_program(VERTEX_SHADER_SOURCE, FONT_SHADER_SOURCE, "font");

    error_check("Renderer::init_window() -> after load shaders");
    return win;
}


NormalisedPoint Renderer::convert_window_to_normal(const WindowPoint &pt)
{
    auto &win = Window::ref();
    float x = float(pt.x)/float(win.get_width())*2.f - 1.f, y = float(pt.y)/float(win.get_height())*2.f - 1.f;
    return {x, y};
}