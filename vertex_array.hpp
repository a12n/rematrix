#ifndef REMATRIX_VERTEX_ARRAY_HPP
#define REMATRIX_VERTEX_ARRAY_HPP

#include <GL/glew.h>

namespace rematrix {

struct vertex_array
{
    vertex_array();

    vertex_array(vertex_array&& other) noexcept;

    ~vertex_array();

    vertex_array&
    operator=(vertex_array&& other) noexcept;

    void
    bind();

    void
    unbind();

    const GLuint id{0};
};

} // namespace rematrix

#endif  // REMATRIX_VERTEX_ARRAY_HPP
