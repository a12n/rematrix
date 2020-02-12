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
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        string log(length, '\0');
        glGetShaderInfoLog(id, length, nullptr, log.data());
        throw runtime_error(log);
    }
}

//----------------------------------------------------------------------------

vertex_shader::vertex_shader() :
    shader{GL_VERTEX_SHADER}
{
}

vertex_shader::vertex_shader(const char* src) :
    vertex_shader()
{
    compile(src);
}

//----------------------------------------------------------------------------

fragment_shader::fragment_shader() :
    shader{GL_FRAGMENT_SHADER}
{
}

fragment_shader::fragment_shader(const char* src) :
    fragment_shader()
{
    compile(src);
}

} // namespace rematrix
