#include "buffer.hpp"

namespace rematrix {

buffer::buffer(GLenum target, const void* data, GLsizeiptr size, GLenum usage)
{
    glGenBuffers(1, const_cast<GLuint*>(&id));
    if (id == 0) {
        throw runtime_error("couldn't generate buffer");
    }
    glBindBuffer(target, id);
    glBufferData(target, size, data, usage);
}

buffer::buffer(buffer&& other) noexcept :
    id{other.id}
{
    const_cast<GLuint&>(other.id) = 0;
}

buffer::~buffer()
{
    glDeleteBuffers(1, &id);
}

buffer&
buffer::operator=(buffer&& other) noexcept
{
    const_cast<GLuint&>(id) = other.id;
    const_cast<GLuint&>(other.id) = 0;
    return *this;
}

} // namespace rematrix
