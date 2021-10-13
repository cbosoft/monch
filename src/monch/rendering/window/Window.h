//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_WINDOW_H
#define MONCH_WINDOW_H


#include <atomic>

#include "monch/rendering/renderable/Renderable.h"


union unicode_char_to_cstr {
    char32_t unicode_codepoint;
    char cstr[sizeof(char32_t)/sizeof(char) + 1];
};


// Forward declarations
class GLFWwindow;


class Window final: public Renderable {
public:
    static Window &ref();
    ~Window() final;

    void run();

    void key_pressed(int key, int mods);
    void key_released(int key, int mods);
    void character_input(char32_t ch);
    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;

    [[nodiscard]] bool has_resized() const;

protected:
    void render_me() final;
    void after_children_rendered() final;

private:
    Window();
    void init();

    GLFWwindow *_glfw_window;

    std::atomic_bool _should_quit, _has_resized;
    int _width, _height;

    friend void window_size_callback(GLFWwindow *win, int wid, int hgt);
};


#endif //MONCH_WINDOW_H
