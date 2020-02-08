#include "program.hpp"
#include "shader.hpp"

namespace rematrix {

program::program() :
    id{glCreateProgram()}
{
    if (id == 0) {
        throw runtime_error("couldn't create program");
    }
}

program::program(const vertex_shader& v, const fragment_shader& f) :
    program()
{
    link(v, f);
}

program::program(program&& other) noexcept :
    id{other.id}
{
    const_cast<GLuint&>(other.id) = 0;
}

program::~program()
{
    glDeleteProgram(id);
}

program&
program::operator=(program&& other) noexcept
{
    const_cast<GLuint&>(id) = other.id;
    const_cast<GLuint&>(other.id) = 0;
    return *this;
}

void
program::link(const vertex_shader& v, const fragment_shader& f)
{
    glAttachShader(id, v.id);
    glAttachShader(id, f.id);
    glLinkProgram(id);
    GLint ok;
    glGetProgramiv(id, GL_LINK_STATUS, &ok);
    glDetachShader(id, f.id);
    glDetachShader(id, v.id);
    if (! ok) {
        throw runtime_error("couldn't link program");
    }
}

} // namespace rematrix
