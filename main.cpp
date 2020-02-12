#include <GL/glew.h>

#include "buffer.hpp"
#include "program.hpp"
#include "rendering_context.hpp"
#include "resources.hpp"
#include "shader.hpp"

using namespace rematrix;

namespace {

unique_ptr<program> prog;
unique_ptr<vertex_buffer> vertex_buf;
unique_ptr<element_buffer> index_buf;


void
init(const pair<unsigned int, unsigned int>& window_size)
{
    if (auto status{glewInit()}; status != GLEW_OK) {
        throw runtime_error(reinterpret_cast<const char*>(glewGetErrorString(status)));
    }

    if (! GLEW_VERSION_3_0) {
        throw runtime_error("OpenGL 3.0 required");
    }

    // Build GLSL program

    prog = make_unique<program>(vertex_shader{vertex_src}, fragment_shader{fragment_src});
    prog->use();

    // Make a rectangle

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

    const GLint position_attrib{prog->attrib_location("position")};

    vertex_buf->bind();
    glVertexAttribPointer(position_attrib,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GLfloat) * 3,
                          static_cast<void*>(0));
    glEnableVertexAttribArray(position_attrib);

    index_buf->bind();

    // Other GL init

    glViewport(0, 0, window_size.first, window_size.second);
    glClearColor(static_cast<double>(0x3c) / 255,
                 static_cast<double>(0x38) / 255,
                 static_cast<double>(0x36) / 255,
                 1.0);
}

void
render(const duration<float>&)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLE_STRIP,
                   4,
                   GL_UNSIGNED_SHORT,
                   static_cast<void*>(0));
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
