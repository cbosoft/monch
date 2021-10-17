//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_EDITORAPP_H
#define MONCH_EDITORAPP_H

#include <thread>
#include <atomic>
#include <monch/events/manager/EventManager.h>
#include <monch/rendering/container/Container.h>


union unicode_char_to_cstr {
    char32_t unicode_codepoint;
    char cstr[sizeof(char32_t)/sizeof(char) + 1];
};


// Forward declarations
class GLFWwindow;
class TextArea;


class EditorApp final: public Container, public EventManager {
    MONCH_OBJECT("EditorApp")
public:
    static EditorApp &ref();
    ~EditorApp() final;

    void run();

    [[nodiscard]] bool has_resized() const;

    void render() final;
    void set_has_changes();

    bool dictates_object_size_position() const override { return true; }
    WindowPoint get_size_of(const Object *object) override;
    WindowPoint get_position_of(const Object *object) override;


private:
    EditorApp();

    void key_pressed(int key, int mods);
    void key_released(int key, int mods);
    void character_input(char32_t ch);
    void resized(int width, int height);

    void post_render() final;

    std::atomic_bool _should_quit, _has_resized, _has_changes;

    friend void window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    friend void window_character_callback(GLFWwindow *window, unsigned int codepoint);
    friend void window_size_callback(GLFWwindow *win, int width, int height);

    std::thread *_event_thread;
    TextArea *_text_area;
    std::chrono::time_point<std::chrono::system_clock> _time_last_render;
};


#endif //MONCH_EDITORAPP_H
