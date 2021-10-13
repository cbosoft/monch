//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_EDITORAPP_H
#define MONCH_EDITORAPP_H

#include <thread>
#include <atomic>
#include <monch/editor/text_area/TextArea.h>

#include "monch/rendering/renderable/Renderable.h"


union unicode_char_to_cstr {
    char32_t unicode_codepoint;
    char cstr[sizeof(char32_t)/sizeof(char) + 1];
};


// Forward declarations
class GLFWwindow;


class EditorApp final: public Renderable {
public:
    static EditorApp &ref();
    ~EditorApp() final;

    void run();

    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;

    [[nodiscard]] bool has_resized() const;

    void event_thread_loop();

protected:
    void render_me() final;
    void after_children_rendered() final;

private:
    EditorApp();

    // editor actions
    void backspace();
    void enter();

    void key_pressed(int key, int mods);
    void key_released(int key, int mods);
    void character_input(char32_t ch);
    void resized(int width, int height);

    std::atomic_bool _should_quit, _has_resized;
    int _width, _height;

    friend void window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    friend void window_character_callback(GLFWwindow *window, unsigned int codepoint);
    friend void window_size_callback(GLFWwindow *win, int width, int height);

    std::thread *_event_thread;
    TextArea *_text_area;
};


#endif //MONCH_EDITORAPP_H
