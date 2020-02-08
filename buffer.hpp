#ifndef REMATRIX_BUFFER_HPP
#define REMATRIX_BUFFER_HPP

#include <GL/glew.h>

#include "decl.hpp"

namespace rematrix {

struct buffer
{
    buffer(GLenum target, const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

    buffer(const buffer&) = delete;

    buffer(buffer&& other) noexcept;

    ~buffer();

    buffer&
    operator=(const buffer&) = delete;

    buffer&
    operator=(buffer&& other) noexcept;

    const GLuint id{0};

protected:
    void
    bind(GLenum target);
};

} // namespace rematrix

#endif  // REMATRIX_BUFFER_HPP
