#include "program.hpp"
#include "shader.hpp"

namespace rematrix {

program::program()
    : id_ { glCreateProgram() }
{
    if (id_ == 0) {
        throw runtime_error("couldn't create program");
    }
}

program::program(const vertex_shader& v, const fragment_shader& f)
    : program()
{
    link(v, f);
}

program::program(program&& other) noexcept
    : id_ { other.id_ }
{
    other.id_ = 0;
}

program::~program()
{
    glDeleteProgram(id_);
}

program& program::operator=(program&& other) noexcept
{
    id_ = other.id_;
    other.id_ = 0;
    return *this;
}

void program::link(const vertex_shader& v, const fragment_shader& f)
{
    glAttachShader(id_, v.id());
    glAttachShader(id_, f.id());
    glLinkProgram(id_);
    GLint ok;
    glGetProgramiv(id_, GL_LINK_STATUS, &ok);
    glDetachShader(id_, f.id());
    glDetachShader(id_, v.id());
    if (!ok) {
        GLint length;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);
        string log(length, '\0');
        glGetProgramInfoLog(id_, length, nullptr, log.data());
        throw runtime_error(log);
    }
}

void program::use()
{
    glUseProgram(id_);
}

void program::bind_attrib_location(GLuint index, const char* name)
{
    glBindAttribLocation(id_, index, name);
}

void program::bind_frag_data_location(GLuint color_number, const char* name)
{
    glBindFragDataLocation(id_, color_number, name);
}

GLint program::attrib_location(const GLchar* name) const
{
    auto ans { glGetAttribLocation(id_, name) };
    if (ans == -1) {
        throw runtime_error("couldn't locate program attribute");
    }
    return ans;
}

GLint program::uniform_location(const GLchar* name) const
{
    auto ans { glGetUniformLocation(id_, name) };
    if (ans == -1) {
        string msg { "couldn't locate \"" };
        msg += name;
        msg += "\" uniform in program";
        throw runtime_error(msg);
    }
    return ans;
}

void program::set_uniform(GLint location, GLint i)
{
    glUniform1i(location, i);
}

void program::set_uniform(GLint location, GLfloat x)
{
    glUniform1f(location, x);
}

void program::set_uniform(GLint location, GLint i1, GLint i2)
{
    glUniform2i(location, i1, i2);
}

void program::set_uniform(GLint location, const vec3& v)
{
    glUniform3fv(location, 1, &v[0]);
}

void program::set_uniform(GLint location, const vec4& v)
{
    glUniform4fv(location, 1, &v[0]);
}

void program::set_uniform(GLint location, const mat4& m)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
}

void program::set_uniform(GLint location, bool b)
{
    glUniform1i(location, b);
}

} // namespace rematrix
