#include "buffer.hpp"

namespace rematrix {

buffer::buffer(GLenum target, const void* data, GLsizeiptr size, GLenum usage)
{
    glGenBuffers(1, &id);
    if (id == 0) {
        throw runtime_error("couldn't generate buffer");
    }
    bind(target);
    glBufferData(target, size, data, usage);
}

buffer::buffer(buffer&& other) noexcept :
    id{other.id}
{
    other.id = 0;
}

buffer::~buffer()
{
    glDeleteBuffers(1, &id);
}

buffer&
buffer::operator=(buffer&& other) noexcept
{
    id = other.id;
    other.id = 0;
    return *this;
}

void
buffer::bind(GLenum target)
{
    glBindBuffer(target, id);
}

//----------------------------------------------------------------------------

vertex_buffer::vertex_buffer(const void* data, GLsizeiptr size, GLenum usage) :
    buffer{GL_ARRAY_BUFFER, data, size, usage}
{
}

void
vertex_buffer::bind()
{
    buffer::bind(GL_ARRAY_BUFFER);
}

} // namespace rematrix
