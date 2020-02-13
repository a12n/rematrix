#include <GL/glew.h>

#include "buffer.hpp"
#include "program.hpp"
#include "rendering_context.hpp"
#include "resources.hpp"
#include "shader.hpp"
#include "texture.hpp"

using namespace rematrix;

namespace {

unique_ptr<program> prog;
unique_ptr<vertex_buffer> position_buf;
unique_ptr<texture> tex;

GLint position_attrib{-1};

void
init(const array<unsigned int, 2>& window_size)
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

    position_attrib = prog->attrib_location("position");

    // Make a rectangle

    const GLfloat vertices[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
        -0.8f,  0.8f, 0.0f,
         0.8f,  0.8f, 0.0f
    };

    position_buf = make_unique<vertex_buffer>(vertices, sizeof(vertices));
    position_buf->bind();
    glVertexAttribPointer(position_attrib,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          static_cast<void*>(0));
    glEnableVertexAttribArray(position_attrib);

    // Load font texture

    tex = make_unique<texture>(GL_R8, font_image_width, font_image_height, GL_RED, GL_UNSIGNED_BYTE, font_image_data);

    // Other GL init

    glViewport(0, 0, window_size[0], window_size[1]);
    glClearColor(static_cast<double>(0x28) / 255,
                 static_cast<double>(0x28) / 255,
                 static_cast<double>(0x28) / 255,
                 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
render(const steady_clock::time_point&)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

} // namespace

int
main()
{
    rendering_context ctx;

    init(ctx.window_size());

    constexpr unsigned int frame_rate{5};
    constexpr milliseconds frame_interval{1'000 / frame_rate};
    auto frame_tick{steady_clock::now()};

    while (true) {
        this_thread::sleep_until(frame_tick += frame_interval);
        if (! ctx.window_obscured()) {
            render(frame_tick);
            ctx.swap_buffers();
        }
    }

    return 0;
}
