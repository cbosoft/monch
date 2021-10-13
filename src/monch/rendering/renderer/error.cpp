//
// Created by Christopher Boyle on 12/10/2021.
//

#include <iostream>

#include "monch/rendering/gl.h"
#include "Renderer.h"

void Renderer::error_check(const std::string &from) const
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        // Process/log the error.
        std::string errtype = "unknown";
        switch (err) {
            case GL_INVALID_ENUM:
                errtype = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:
                errtype = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:
                errtype = "GL_INVALID_OPERATION"; break;
//            case GL_STACK_OVERFLOW:
//                errtype = "GL_STACK_OVERFLOW"; break;
//            case GL_STACK_UNDERFLOW:
//                errtype = "GL_STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:
                errtype = "GL_OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errtype = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
//            case GL_CONTEXT_LOST:
//                errtype = "GL_CONTEXT_LOST"; break;

            default:
                break;
        }
        std::cerr << "OpenGL error " << errtype << " found " << from << "\n";
        throw std::runtime_error("OpenGL error encountered.");
    }
}