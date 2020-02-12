#ifndef REMATRIX_TEXTURE_HPP
#define REMATRIX_TEXTURE_HPP

#include <GL/glew.h>

#include "decl.hpp"

namespace rematrix {

struct texture
{
    texture();

    texture(const texture&) = delete;

    texture(texture&& other) noexcept;

    ~texture();

    texture&
    operator=(const texture&) = delete;

    texture&
    operator=(texture&& other) noexcept;

    void
    load(GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);

    const GLuint id{0};
};

} // namespace rematrix

#endif  // REMATRIX_TEXTURE_HPP
