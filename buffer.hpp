#ifndef REMATRIX_BUFFER_HPP
#define REMATRIX_BUFFER_HPP

#include <GL/glew.h>

#include "decl.hpp"

namespace rematrix {

struct buffer
{
    buffer(GLenum target, const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

    buffer(buffer&& other) noexcept;

    ~buffer();

    buffer&
    operator=(buffer&& other) noexcept;

    void
    bind(GLenum target);

private:
    GLuint id { 0 };
};

//----------------------------------------------------------------------------

struct vertex_buffer : buffer
{
    vertex_buffer(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

    void
    bind();
};

} // namespace rematrix

#endif  // REMATRIX_BUFFER_HPP
