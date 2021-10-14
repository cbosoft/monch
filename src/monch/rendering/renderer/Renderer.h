//
// Created by Christopher Boyle on 12/10/2021.
//

#ifndef MONCH_RENDERER_H
#define MONCH_RENDERER_H

#include <map>
#include <string>

#include <monch/util.h>
#include <monch/rendering/gl.h>
#include <monch/rendering/vertex_buffer/VertexBuffer.h>
#include <monch/object/Object.h>

void window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void window_character_callback(GLFWwindow *window, unsigned int codepoint);
void window_size_callback(GLFWwindow *window, int width, int height);

class Renderable;
class Renderer {
public:
    static Renderer &ref();
    ~Renderer() =default;
    Renderer(const Renderer &) =delete;
    Renderer &operator=(const Renderer &) =delete;

    void init();

    void swap_and_poll() const;

    void use_assigned_shader(Renderable *rbl);
    void assign_shader(Renderable *rbl, const std::string &name);

    void error_check(const std::string &from) const;

    static NormalisedPoint convert_window_to_normal(const WindowPoint &pt);

    void get_window_size(int &width, int &height) const;

    void render(Object *obj);

private:

    Renderer();

    void load_shader_program(const std::string &vertex_source, const std::string &fragment_source, const std::string &name);
    static uint compile_shader(const std::string &source, int shader_type);
    static uint compile_vertex(const std::string &source);
    static uint compile_fragment(const std::string &source);

    static void use_shader(uint shader);

    std::map<Renderable *, uint> _assigned_shaders;
    std::map<std::string, uint> _shaders;

    GLFWwindow *_glfw_window;
    int _width, _height;

    // TODO: merge rendering stuff from window into here
};


#endif //MONCH_RENDERER_H
