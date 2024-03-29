#ifndef REMATRIX_FRAME_BUFFER_HPP
#define REMATRIX_FRAME_BUFFER_HPP

#include <GL/glew.h>

#include "decl.hpp"

namespace rematrix {

struct frame_buffer
{
    frame_buffer();

    frame_buffer(frame_buffer&& other) noexcept;

    ~frame_buffer();

    frame_buffer&
    operator=(frame_buffer&& other) noexcept;

    void
    bind();

    void
    unbind();

    void
    draw_buffers(initializer_list<GLenum> bufs);

    void
    attach(GLenum point, GLuint texture_id);

    void
    check_complete() const;

    const GLuint id{0};
};

} // namespace rematrix

#endif  // REMATRIX_FRAME_BUFFER_HPP
