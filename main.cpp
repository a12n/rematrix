#include <GL/glew.h>

#include "buffer.hpp"
#include "matrix.hpp"
#include "program.hpp"
#include "rendering_context.hpp"
#include "resources.hpp"
#include "shader.hpp"
#include "texture.hpp"

using namespace rematrix;

namespace {

unique_ptr<program> prog;
unique_ptr<vertex_buffer> vertex_buf;
unique_ptr<texture> tex;

GLint position_attrib{-1};
GLint tex_coord_attrib{-1};

GLint model_uniform{-1};
GLint projection_uniform{-1};
GLint view_uniform{-1};

GLint char_pos_uniform{-1};
GLint feeder_pos_uniform{-1};
GLint is_erasing_uniform{-1};
GLint is_feeder_uniform{-1};

minstd_rand rand{0};

vector<unsigned int> glyph_indices;

void
render_glyph(unsigned int index)
{
    glDrawArrays(GL_TRIANGLE_STRIP, index, 4);
    // glBegin(GL_LINE_STRIP);
    // glVertex2f(-0.5f, -0.5f);
    // glVertex2f( 0.5f, -0.5f);
    // glVertex2f( 0.5f,  0.5f);
    // glVertex2f(-0.5f,  0.5f);
    // glVertex2f(-0.5f, -0.5f);
    // glEnd();
}

//----------------------------------------------------------------------------

struct strip
{
    static constexpr unsigned int grid_size{70};
    static constexpr unsigned int grid_depth{35};
    static constexpr float splash_ratio{0.7f};

    void
    reset()
    {
        uniform_int_distribution<unsigned long> glyph_indices_distr{0, glyph_indices.size() - 1};

        position[0] = uniform_real_distribution{-(grid_size / 2.0f), grid_size / 2.0f}(rand);
        position[1] = normal_distribution{grid_size / 2.0f}(rand);
        position[2] = uniform_real_distribution{-(grid_depth * 0.75f), grid_depth * 0.2f}(rand);

        velocity[0] = 0.0f;
        velocity[1] = 0.0f;
        velocity[2] = max(normal_distribution{0.8f, 0.01f}(rand), 0.001f);

        erasing = false;

        feeder_char = glyph_indices[glyph_indices_distr(rand)];
        feeder_y = 0.0f;
        feeder_speed = max(normal_distribution{3.0f}(rand), 0.001f);

        spin_accum = 0.0f;
        spin_after = max(normal_distribution{1.0f, 0.3f}(rand), 0.001f);

        for (auto& [index, spin] : chars) {
            index = glyph_indices[glyph_indices_distr(rand)];
            spin = bernoulli_distribution{1.0 / 20.0}(rand);
        }
    }

    void
    update(const duration<double>& dt)
    {
        position[0] += velocity[0] * dt.count();
        position[1] += velocity[1] * dt.count();
        position[2] += velocity[2] * dt.count();

        if (position[2] > splash_ratio * grid_depth) {
            reset();
            return;
        }

        feeder_y += feeder_speed * dt.count();
        if (feeder_y >= grid_size) {
            if (erasing) {
                reset();
                return;
            } else {
                erasing = true;
                feeder_y = 0.0f;
                feeder_speed /= 2.0f;
            }
        }

        spin_accum += dt.count();
        if (spin_accum >= spin_after) {
            uniform_int_distribution<unsigned long> glyph_indices_distr{0, glyph_indices.size() - 1};

            spin_accum = 0.0f;
            feeder_char = glyph_indices[glyph_indices_distr(rand)];
            for (auto& [index, spin] : chars) {
                if (spin) {
                    index = glyph_indices[glyph_indices_distr(rand)];
                    spin = bernoulli_distribution{799.0f / 800.0f}(rand);
                }
            }
        }
    }

    void
    render() const
    {
        prog->set_uniform(is_feeder_uniform, false);
        prog->set_uniform(is_erasing_uniform, erasing);
        prog->set_uniform(feeder_pos_uniform, feeder_y);
        for (unsigned int i{0}; i < chars.size(); ++i) {
            bool below{feeder_y > i};

            if (erasing) {
                below = ! below;
            }

            if (below) {
                prog->set_uniform(char_pos_uniform, static_cast<int>(i));
                prog->set_uniform(model_uniform, translate(mat4(1.0f), {position[0], position[1] - i, position[2]}));
                render_glyph(chars[i].first);
            }
        }

        if (! erasing) {
            prog->set_uniform(model_uniform, translate(mat4(1.0f), {position[0], position[1] - feeder_y, position[2]}));
            prog->set_uniform(is_feeder_uniform, true);
            render_glyph(feeder_char);
        }
    }

    array<float, 3> position;
    array<float, 3> velocity;

    bool erasing;

    unsigned int feeder_char;
    float feeder_y;
    float feeder_speed;

    array<pair<unsigned int, bool>, grid_size> chars;

    float spin_accum;
    float spin_after;
};

vector<strip> strips;

//----------------------------------------------------------------------------

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

    char_pos_uniform = prog->uniform_location("charPos");
    feeder_pos_uniform = prog->uniform_location("feederPos");
    is_erasing_uniform = prog->uniform_location("isErasing");
    is_feeder_uniform = prog->uniform_location("isFeeder");

    {
        const auto ar = static_cast<float>(window_size[0]) / static_cast<float>(window_size[1]);
        prog->set_uniform(projection_uniform, perspective(radians(80.0f), ar, 0.1f, 100.0f));
        prog->set_uniform(view_uniform, translate(mat4(1.0f), {0.0f, 0.0f, -25.0f}));
    }

    // Make vertex buffer

    {
        const auto [v, i] = font.buffer_data();

        // Create and fill buffer

        vertex_buf = make_unique<vertex_buffer>(v.data(), v.size() * sizeof(float));
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

        // Keep list of known glyph indices

        for (const auto [c, j] : i) {
            glyph_indices.push_back(j);
        }
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

    // Create strips

    strips.resize(100);
    for (strip& s : strips) {
        s.reset();
    }
}

void
update(const duration<double>& dt)
{
    // TODO: parallel?
    for (strip& s : strips) {
        s.update(dt);
    }
}

void
render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (const strip& s : strips) {
        s.render();
    }
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
        update(frame_interval);
        if (! ctx.window_obscured()) {
            render();
            ctx.swap_buffers();
        }
    }

    return 0;
}
