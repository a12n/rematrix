#ifndef REMATRIX_PROGRAM_HPP
#define REMATRIX_PROGRAM_HPP

#include <GL/glew.h>

#include "decl.hpp"

namespace rematrix {

struct program
{
    program();

    program(const vertex_shader& v, const fragment_shader& f);

    program(const program&) = delete;

    program(program&& other) noexcept;

    ~program();

    program&
    operator=(const program&) = delete;

    program&
    operator=(program&& other) noexcept;

    void
    link(const vertex_shader& v, const fragment_shader& f);

    void
    use();

    GLint
    attrib_location(const GLchar* name) const;

    GLint
    uniform_location(const GLchar* name) const;

    const GLuint id;
};

} // namespace rematrix

#endif  // REMATRIX_PROGRAM_HPP
