#include <GL/glew.h>

#include "buffer.hpp"
#include "program.hpp"
#include "rendering_context.hpp"
#include "shader.hpp"

using namespace rematrix;

namespace {

unique_ptr<program> prog;
unique_ptr<vertex_buffer> vertex_buf;
unique_ptr<element_buffer> index_buf;

GLint position_attrib;

void
init(const pair<unsigned int, unsigned int>& window_size)
{
    if (auto status{glewInit()}; status != GLEW_OK) {
        throw runtime_error(reinterpret_cast<const char*>(glewGetErrorString(status)));
    }

    if (! GLEW_VERSION_3_0) {
        throw runtime_error("OpenGL 3.0 required");
    }

    const GLfloat vertices[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
        -0.8f,  0.8f, 0.0f,
         0.8f,  0.8f, 0.0f
    };
    const GLushort indices[] = {
        0, 1, 2, 3
    };

    vertex_buf = make_unique<vertex_buffer>(vertices, sizeof(vertices));
    index_buf = make_unique<element_buffer>(indices, sizeof(indices));

    const vertex_shader v_shader{R"(
#version 130

attribute vec3 position;

void
main()
{
    gl_Position = vec4(position, 1.0);
}
)"};

    const fragment_shader f_shader{R"(
#version 130

void
main()
{
    gl_FragColor = vec4(0.1, 0.2, 0.3, 1.0);
}
)"};

    prog = make_unique<program>(v_shader, f_shader);
    position_attrib = prog->attrib_location("position");
    prog->use();

    glViewport(0, 0, window_size.first, window_size.second);

    glClearColor(0, 0, 0, 1);
}

void
render(const duration<float>& dt)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace

int
main()
{
    rendering_context ctx;

    init(ctx.window_size());

    constexpr unsigned int frame_rate{5};
    constexpr duration<float> frame_interval{1.0f / frame_rate};
    auto prev_time{steady_clock::now()};

    while (true) {
        this_thread::sleep_for(frame_interval);
        auto cur_time{steady_clock::now()};
        render(duration_cast<duration<float>>(cur_time - prev_time));
        ctx.swap_buffers();
        prev_time = cur_time;
    }

    return 0;
}
