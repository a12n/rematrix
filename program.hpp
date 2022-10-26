#ifndef REMATRIX_PROGRAM_HPP
#define REMATRIX_PROGRAM_HPP

#include <GL/glew.h>

#include "decl.hpp"
#include "matrix.hpp"

namespace rematrix {

struct program {
    program();

    program(const vertex_shader& v, const fragment_shader& f);

    program(program&& other) noexcept;

    ~program();

    program& operator=(program&& other) noexcept;

    void link(const vertex_shader& v, const fragment_shader& f);

    void use();

    void bind_attrib_location(GLuint index, const char* name);

    void bind_frag_data_location(GLuint color_number, const char* name);

    GLint attrib_location(const GLchar* name) const;

    GLint uniform_location(const GLchar* name) const;

    void set_uniform(GLint location, GLint i);

    void set_uniform(GLint location, GLfloat x);

    void set_uniform(GLint location, GLint i1, GLint i2);

    void set_uniform(GLint location, const vec3& v);

    void set_uniform(GLint location, const vec4& v);

    void set_uniform(GLint location, const mat4& m);

    void set_uniform(GLint location, bool b);

private:
    GLuint id_;
};

} // namespace rematrix

#endif // REMATRIX_PROGRAM_HPP
