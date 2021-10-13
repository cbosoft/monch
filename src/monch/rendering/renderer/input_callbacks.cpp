//
// Created by Christopher Boyle on 13/10/2021.
//

#include "../window/Window.h"
#include "Renderer.h"


void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

void window_character_callback(GLFWwindow* window, unsigned int codepoint)
{
    (void) window;
    Window::ref().character_input(codepoint);
}


void window_size_callback(GLFWwindow *notused, int width, int height)
{
    (void) notused;
    Window::ref().resized(width, height);
}