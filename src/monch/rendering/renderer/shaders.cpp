//
// Created by Christopher Boyle on 12/10/2021.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "monch/rendering/gl.h"

#include "Renderer.h"


uint Renderer::compile_shader(const std::string &source, int shader_type)
{
    uint shader_id = glCreateShader(shader_type);
    char const *c_source = source.c_str();
    glShaderSource(shader_id, 1, &c_source, nullptr);
    glCompileShader(shader_id);

    int result = 0, loglen = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0) {
        std::vector<char> log(loglen+1);
        glGetShaderInfoLog(shader_id, loglen, nullptr, log.data());
        std::cerr << source << ": " << log.data() << "\n";
        if (!result)
            return -1;
    }
    return shader_id;
}


uint Renderer::compile_vertex(const std::string &source)
{
    return compile_shader(source, GL_VERTEX_SHADER);
}


uint Renderer::compile_fragment(const std::string &source)
{
    return compile_shader(source, GL_FRAGMENT_SHADER);
}


void Renderer::load_shader_program(const std::string &vertex_source, const std::string &fragment_source, const std::string &name)
{
    // Logger::ref() << LL_INFO << "Loading shaders \"" << vertex_name << "\" and \"" << fragment_name << "\"\n";
    uint vertex_id = this->compile_vertex(vertex_source);
    uint fragment_id = this->compile_fragment(fragment_source);
    uint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
    glLinkProgram(program_id);
    int result = 0, loglen = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0 && !result) {
        std::vector<char> log(loglen+1);
        glGetProgramInfoLog(program_id, loglen, nullptr, log.data());
        std::cerr << log.data();
        if (!result) {
            // TODO error?
        }
    }

    glDetachShader(program_id, vertex_id);
    glDetachShader(program_id, fragment_id);

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    _shaders[name] = program_id;
}


void Renderer::use_assigned_shader(Renderable *rbl)
{
    auto it = _assigned_shaders.find(rbl);
    if (it == _assigned_shaders.end()) {
        // TODO error?
        std::cerr << "no assigned shader" << std::endl;
        return;
    }

    use_shader(it->second);
}

void Renderer::assign_shader(Renderable *rbl, const std::string &name)
{
    auto it = _shaders.find(name);
    if (it == _shaders.end()) {
        // TODO: error?
        std::cerr << "could not assign shader, none found with name " << name << std::endl;
    }
    else {
        _assigned_shaders[rbl] = it->second;
    }
}

void Renderer::use_shader(uint shader)
{
    glUseProgram(shader);
}