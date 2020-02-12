#include "texture.hpp"

namespace rematrix {

texture::texture()
{
    glGenTextures(1, const_cast<GLuint*>(&id));
    if (id == 0) {
        throw runtime_error("couldn't create texture");
    }
}

texture::texture(GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels) :
    texture()
{
    load(internal_format, width, height, format, type, pixels);
}

texture::texture(texture&& other) noexcept :
    id{other.id}
{
    const_cast<GLuint&>(other.id) = 0;
}

texture::~texture()
{
    glDeleteTextures(1, &id);
}

texture&
texture::operator=(texture&& other) noexcept
{
    const_cast<GLuint&>(id) = other.id;
    const_cast<GLuint&>(other.id) = 0;
    return *this;
}

void
texture::load(GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
        GL_TEXTURE_2D, 0,         // target, level of detail
        internal_format,          // internal format
        width, height, 0,         // width, height, border
        format, type,             // external format, type
        pixels                    // pixels
        );
}

} // namespace rematrix
