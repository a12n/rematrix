#include "decl.hpp"
#include "frame_buffer.hpp"

namespace rematrix {

frame_buffer::frame_buffer()
{
    glGenFramebuffers(1, &id);
    if (id == 0) {
        throw runtime_error("couldn't generate frame buffer");
    }
}

frame_buffer::frame_buffer(frame_buffer&& other) noexcept :
    id{other.id}
{
    other.id = 0;
}

frame_buffer::~frame_buffer()
{
    glDeleteFramebuffers(1, &id);
}

frame_buffer&
frame_buffer::operator=(frame_buffer&& other) noexcept
{
    id = other.id;
    other.id = 0;
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
frame_buffer::draw_buffers(initializer_list<GLenum> bufs)
{
    const vector<GLenum> bufs2{bufs};
    glDrawBuffers(bufs2.size(), bufs2.data());
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
