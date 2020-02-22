#include "decl.hpp"
#include "vertex_array.hpp"

namespace rematrix {

vertex_array::vertex_array()
{
    glGenVertexArrays(1, const_cast<GLuint*>(&id));
    if (id == 0) {
        throw runtime_error("couldn't create vertex array");
    }
}

vertex_array::vertex_array(vertex_array&& other) noexcept :
    id{other.id}
{
    const_cast<GLuint&>(other.id) = 0;
}

vertex_array::~vertex_array()
{
    glDeleteVertexArrays(1, &id);
}

vertex_array&
vertex_array::operator=(vertex_array&& other) noexcept
{
    const_cast<GLuint&>(id) = other.id;
    const_cast<GLuint&>(other.id) = 0;
    return *this;
}

void
vertex_array::bind()
{
    glBindVertexArray(id);
}

} // namespace rematrix
