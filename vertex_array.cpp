#include "decl.hpp"
#include "vertex_array.hpp"

namespace rematrix {

vertex_array::vertex_array()
{
    glGenVertexArrays(1, &id_);
    if (id_ == 0) {
        throw runtime_error("couldn't create vertex array");
    }
}

vertex_array::vertex_array(vertex_array&& other) noexcept :
    id_{other.id_}
{
    other.id_ = 0;
}

vertex_array::~vertex_array()
{
    glDeleteVertexArrays(1, &id_);
}

vertex_array&
vertex_array::operator=(vertex_array&& other) noexcept
{
    id_ = other.id_;
    other.id_ = 0;
    return *this;
}

void
vertex_array::bind()
{
    glBindVertexArray(id_);
}

void
vertex_array::unbind()
{
    glBindVertexArray(0);
}

} // namespace rematrix
