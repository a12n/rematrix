#include "decl.hpp"
#include "frame_buffer.hpp"

namespace rematrix {

frame_buffer::frame_buffer()
{
    glGenFramebuffers(1, const_cast<GLuint*>(&id));
    if (id == 0) {
        throw runtime_error("couldn't generate frame buffer");
    }
}

frame_buffer::frame_buffer(frame_buffer&& other) noexcept :
    id{other.id}
{
    const_cast<GLuint&>(other.id) = 0;
}

frame_buffer::~frame_buffer()
{
    glDeleteFramebuffers(1, &id);
}

frame_buffer&
frame_buffer::operator=(frame_buffer&& other) noexcept
{
    const_cast<GLuint&>(id) = other.id;
    const_cast<GLuint&>(other.id) = 0;
    return *this;
}

void
frame_buffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void
frame_buffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
frame_buffer::attach(GLenum point, GLuint texture_id)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           point,
                           GL_TEXTURE_2D,
                           texture_id,
                           0);
}

void
frame_buffer::check_complete() const
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw runtime_error("frame buffer isn't complete");
    }
}

} // namespace rematrix
