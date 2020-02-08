#include "shader.hpp"

namespace rematrix {

shader::shader(GLenum type) :
    id{glCreateShader(type)}
{
    if (id == 0) {
        throw runtime_error("couldn't create shader");
    }
}

shader::shader(shader&& other) noexcept :
    id{other.id}
{
    const_cast<GLuint&>(other.id) = 0;
}

shader::~shader()
{
    glDeleteShader(id);
}

shader&
shader::operator=(shader&& other) noexcept
{
    const_cast<GLuint&>(id) = other.id;
    const_cast<GLuint&>(other.id) = 0;
    return *this;
}

void
shader::compile(const char* src)
{
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    GLint ok;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (! ok) {
        throw runtime_error("couldn't compile shader");
    }
}

} // namespace rematrix
