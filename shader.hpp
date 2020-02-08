#ifndef REMATRIX_SHADER_HPP
#define REMATRIX_SHADER_HPP

#include <GL/glew.h>

#include "decl.hpp"

namespace rematrix {

struct shader
{
    shader(GLenum type);

    shader(const shader&) = delete;

    shader(shader&& other) noexcept;

    ~shader();

    shader&
    operator=(const shader&) = delete;

    shader&
    operator=(shader&& other) noexcept;

    void
    compile(const char* src);

    const GLuint id;
};

} // namespace rematrix

#endif  // REMATRIX_SHADER_HPP
