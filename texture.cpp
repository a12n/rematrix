#include "texture.hpp"

namespace rematrix {

texture::texture()
{
    glGenTextures(1, &id_);
    if (id_ == 0) {
        throw runtime_error("couldn't create texture");
    }
}

texture::texture(GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels) :
    texture()
{
    load(internal_format, width, height, format, type, pixels);
}

texture::texture(texture&& other) noexcept :
    id_{other.id_}
{
    other.id_ = 0;
}

texture::~texture()
{
    glDeleteTextures(1, &id_);
}

texture&
texture::operator=(texture&& other) noexcept
{
    id_ = other.id_;
    other.id_ = 0;
    return *this;
}

void
texture::load(GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
    bind();
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

void
texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

void
texture::bind(GLenum unit)
{
    glActiveTexture(unit);
    bind();
}

void
texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void
texture::unbind(GLenum unit)
{
    glActiveTexture(unit);
    unbind();
}

} // namespace rematrix
