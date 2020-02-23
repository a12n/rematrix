#ifndef REMATRIX_TEXTURE_HPP
#define REMATRIX_TEXTURE_HPP

#include <GL/glew.h>

#include "decl.hpp"

namespace rematrix {

struct texture
{
    texture();

    texture(GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels = nullptr);

    texture(texture&& other) noexcept;

    ~texture();

    texture&
    operator=(texture&& other) noexcept;

    void
    load(GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels = nullptr);

    void
    bind();

    void
    unbind();

    const GLuint id{0};
};

} // namespace rematrix

#endif  // REMATRIX_TEXTURE_HPP
