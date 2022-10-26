#ifndef REMATRIX_SHADER_HPP
#define REMATRIX_SHADER_HPP

#include <GL/glew.h>

#include "decl.hpp"

namespace rematrix {

struct shader {
    shader(GLenum type, const char* src = nullptr);

    shader(shader&& other) noexcept;

    ~shader();

    shader& operator=(shader&& other) noexcept;

    void compile(const char* src);

    GLuint id() const
    {
        return id_;
    }

private:
    GLuint id_ { 0 };
};

//----------------------------------------------------------------------------

struct vertex_shader : shader {
    vertex_shader(const char* src = nullptr);
};

//----------------------------------------------------------------------------

struct fragment_shader : shader {
    fragment_shader(const char* src = nullptr);
};

} // namespace rematrix

#endif // REMATRIX_SHADER_HPP
