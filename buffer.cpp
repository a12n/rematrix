#include "buffer.hpp"

namespace rematrix {

buffer::buffer(GLenum target, const void* data, GLsizeiptr size, GLenum usage)
{
    glGenBuffers(1, &id_);
    if (id_ == 0) {
        throw runtime_error("couldn't generate buffer");
    }
    bind(target);
    glBufferData(target, size, data, usage);
}

buffer::buffer(buffer&& other) noexcept
    : id_ { other.id_ }
{
    other.id_ = 0;
}

buffer::~buffer()
{
    glDeleteBuffers(1, &id_);
}

buffer& buffer::operator=(buffer&& other) noexcept
{
    id_ = other.id_;
    other.id_ = 0;
    return *this;
}

void buffer::bind(GLenum target)
{
    glBindBuffer(target, id_);
}

//----------------------------------------------------------------------------

vertex_buffer::vertex_buffer(const void* data, GLsizeiptr size, GLenum usage)
    : buffer { GL_ARRAY_BUFFER, data, size, usage }
{
}

void vertex_buffer::bind()
{
    buffer::bind(GL_ARRAY_BUFFER);
}

} // namespace rematrix
