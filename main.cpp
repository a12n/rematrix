#include <GL/glew.h>

#include "buffer.hpp"
#include "matrix.hpp"
#include "program.hpp"
#include "rendering_context.hpp"
#include "resources.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

using namespace rematrix;

namespace {

unique_ptr<program> prog;
unique_ptr<vertex_buffer> vertex_buf;
unique_ptr<texture> tex;

unordered_map<char, unsigned int> vertex_buf_indices;

GLint position_attrib{-1};
GLint tex_coord_attrib{-1};

GLint model_uniform{-1};
GLint projection_uniform{-1};
GLint view_uniform{-1};

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
    tex_coord_attrib = prog->attrib_location("texCoord");

    model_uniform = prog->uniform_location("model");
    projection_uniform = prog->uniform_location("projection");
    view_uniform = prog->uniform_location("view");

    {
        const auto ar = static_cast<float>(window_size[0]) / static_cast<float>(window_size[1]);
        prog->set_uniform(projection_uniform, perspective(radians(45.0f), ar, 0.001f, 100.0f));
        prog->set_uniform(view_uniform, translate(mat4(1.0f), {0.0f, 0.0f, -10.0f}));
    }

    // Make vertex buffer

    {
        const auto [data, indices] = font.data();

        vertex_buf = make_unique<vertex_buffer>(data.data(), data.size() * sizeof(float));
        vertex_buf->bind();

        glVertexAttribPointer(position_attrib,
                              2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(float),
                              static_cast<const void*>(0));
        glEnableVertexAttribArray(position_attrib);

        glVertexAttribPointer(tex_coord_attrib,
                              2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(float),
                              reinterpret_cast<const void*>(static_cast<uintptr_t>(2 * sizeof(float))));
        glEnableVertexAttribArray(tex_coord_attrib);

        vertex_buf_indices = indices;
    }

    // Load font texture

    tex = make_unique<texture>(GL_R8, font_image_size[0], font_image_size[1], GL_RED, GL_UNSIGNED_BYTE, font_image_data);

    // Other GL init

    glViewport(0, 0, window_size[0], window_size[1]);
    glClearColor(static_cast<double>(0x28) / 255,
                 static_cast<double>(0x28) / 255,
                 static_cast<double>(0x28) / 255,
                 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLfloat angle{0.0f};

void
render(const duration<double>& dt)
{
    glClear(GL_COLOR_BUFFER_BIT);

    angle += 2.0f * dt.count();

    glVertexAttribPointer(tex_coord_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          reinterpret_cast<const void*>(tex_coord_buf_offset.at('A')));
    prog->set_uniform(model_uniform, rotate(translate(mat4(1.0f), {-1.5f, 0.0f, angle / 5.0f}), radians(angle), {0.0f, 0.0f, 1.0f}));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glVertexAttribPointer(tex_coord_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          reinterpret_cast<const void*>(tex_coord_buf_offset.at('C')));
    prog->set_uniform(model_uniform, rotate(translate(mat4(1.0f), {-0.0f, 0.0f, angle / 5.0f}), radians(angle * 2), {0.0f, 0.0f, 1.0f}));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glVertexAttribPointer(tex_coord_attrib, 2, GL_FLOAT, GL_FALSE, 0,
                          reinterpret_cast<const void*>(tex_coord_buf_offset.at('G')));
    prog->set_uniform(model_uniform, rotate(translate(mat4(1.0f), {1.5f, 0.0f, angle / 5.0f}), radians(angle * 3), {0.0f, 0.0f, 1.0f}));
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
            render(frame_interval);
            ctx.swap_buffers();
        }
    }

    return 0;
}
