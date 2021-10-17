//
// Created by Christopher Boyle on 13/10/2021.
//

#include <iostream>
#include <monch/editor/app/EditorApp.h>
#include "Renderer.h"


void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) window;
    (void) scancode;
    switch (action) {
        case GLFW_PRESS:
            EditorApp::ref().key_pressed(key, mods);
            break;
        case GLFW_RELEASE:
            EditorApp::ref().key_released(key, mods);
            break;
        default:
        case GLFW_REPEAT:
            break;
    }
}

void window_character_callback(GLFWwindow* window, unsigned int codepoint)
{
    (void) window;
    EditorApp::ref().character_input(codepoint);
}


void window_size_callback(GLFWwindow *notused, int width, int height)
{
    (void) notused;
    EditorApp::ref().resized(width, height);
}


void window_scroll_callback(GLFWwindow *win, double dx, double dy)
{
    (void) win;
    EditorApp::ref().scrolled(dx, -dy);
}