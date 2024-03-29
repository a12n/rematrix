#include "shader.hpp"

namespace rematrix {

shader::shader(GLenum type, const char* src)
    : id_ { glCreateShader(type) }
{
    if (id_ == 0) {
        throw runtime_error("couldn't create shader");
    }
    if (src) {
        compile(src);
    }
}

shader::shader(shader&& other) noexcept
    : id_ { other.id_ }
{
    other.id_ = 0;
}

shader::~shader()
{
    glDeleteShader(id_);
}

shader& shader::operator=(shader&& other) noexcept
{
    id_ = other.id_;
    other.id_ = 0;
    return *this;
}

void shader::compile(const char* src)
{
    glShaderSource(id_, 1, &src, nullptr);
    glCompileShader(id_);
    GLint ok;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint length;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &length);
        string log(length, '\0');
        glGetShaderInfoLog(id_, length, nullptr, log.data());
        throw runtime_error(log);
    }
}

//----------------------------------------------------------------------------

vertex_shader::vertex_shader(const char* src)
    : shader { GL_VERTEX_SHADER, src }
{
}

//----------------------------------------------------------------------------

fragment_shader::fragment_shader(const char* src)
    : shader { GL_FRAGMENT_SHADER, src }
{
}

} // namespace rematrix
