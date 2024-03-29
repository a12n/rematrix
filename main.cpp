#include <csignal>

#include <GL/glew.h>

#include "buffer.hpp"
#include "matrix.hpp"
#include "options.hpp"
#include "program.hpp"
#include "render_context.hpp"
#include "resources.hpp"
#include "shader.hpp"
#include "texture.hpp"

using namespace rematrix;

namespace {

unique_ptr<program> prog;
unique_ptr<vertex_buffer> font_vertex_buf;
unique_ptr<texture> font_tex;

GLint model_uniform { -1 };

GLint char_pos_uniform { -1 };
GLint feeder_pos_uniform { -1 };
GLint is_erasing_uniform { -1 };
GLint is_feeder_uniform { -1 };
GLint wave_pos_uniform { -1 };

minstd_rand rand { 0 };

vector<unsigned int> glyph_indices;

constexpr GLuint position_attrib { 0 };
constexpr GLuint tex_coord_attrib { 1 };

constexpr GLuint frag_color_attrib { 0 };

void render_glyph(unsigned int index)
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

class strip {
public:
    bool operator<(const strip& other) const
    {
        return position[2] < other.position[2];
    }

    void reset()
    {
        internal_reset();
        erasing = true;
        feeder_pos = uniform_real_distribution { 0.0f, static_cast<float>(grid_size) }(rand);
        for (auto& [index, spin] : chars) {
            index = glyph_indices[0];
            spin = false;
        }
    }

    void update(const duration<double>& dt)
    {
        position[0] += velocity[0] * dt.count();
        position[1] += velocity[1] * dt.count();
        position[2] += velocity[2] * dt.count();

        if (position[2] > splash_ratio * grid_depth) {
            internal_reset();
            return;
        }

        feeder_pos += feeder_speed * dt.count();
        if (feeder_pos >= grid_size) {
            if (erasing) {
                internal_reset();
                return;
            } else {
                erasing = true;
                feeder_pos = 0.0f;
                feeder_speed /= 2.0f;
            }
        }

        spin_accum += dt.count();
        if (spin_accum >= spin_after) {
            spin_accum = 0.0f;

            uniform_int_distribution<unsigned long> glyph_indices_distr { 1, glyph_indices.size() - 1 };

            feeder_char = glyph_indices[glyph_indices_distr(rand)];
            for (auto& [index, spin] : chars) {
                if (spin) {
                    index = glyph_indices[glyph_indices_distr(rand)];
                    spin = bernoulli_distribution { 799.0f / 800.0f }(rand);
                }
            }
        }

        wave_accum += dt.count();
        if (wave_accum >= wave_after) {
            wave_after = 0.0f;
            if (++wave_pos >= wave_size) {
                wave_pos = 0;
            }
        }
    }

    void render() const
    {
        prog->set_uniform(is_feeder_uniform, false);
        prog->set_uniform(is_erasing_uniform, erasing);
        prog->set_uniform(feeder_pos_uniform, feeder_pos);
        prog->set_uniform(wave_pos_uniform, static_cast<int>(wave_pos));
        for (unsigned int i { 0 }; i < chars.size(); ++i) {
            bool below { feeder_pos > i };

            if (erasing) {
                below = !below;
            }

            if (below) {
                prog->set_uniform(char_pos_uniform, static_cast<int>(i));
                prog->set_uniform(model_uniform, translate({ position[0], position[1] - i, position[2] }));
                render_glyph(chars[i].first);
            }
        }

        if (!erasing) {
            prog->set_uniform(model_uniform, translate({ position[0], position[1] - feeder_pos, position[2] }));
            prog->set_uniform(is_feeder_uniform, true);
            render_glyph(feeder_char);
        }
    }

private:
    static constexpr unsigned int grid_size { 70 };
    static constexpr unsigned int grid_depth { 35 };
    static constexpr float splash_ratio { 0.7f };
    static constexpr unsigned int wave_size { 22 };

    void internal_reset()
    {
        uniform_int_distribution<unsigned long> glyph_indices_distr { 1, glyph_indices.size() - 1 };

        position[0] = uniform_real_distribution { -(grid_size / 2.0f), grid_size / 2.0f }(rand);
        position[1] = normal_distribution { grid_size / 2.0f }(rand);
        position[2] = uniform_real_distribution { -(grid_depth * 0.5f), grid_depth * 0.2f }(rand);

        velocity[0] = 0.0f;
        velocity[1] = 0.0f;
        velocity[2] = max(normal_distribution { 0.8f, 0.01f }(rand), 0.001f);

        erasing = false;

        feeder_char = glyph_indices[glyph_indices_distr(rand)];
        feeder_pos = 0.0f;
        feeder_speed = max(normal_distribution { 2.0f }(rand), 0.1f);

        spin_accum = 0.0f;
        spin_after = max(normal_distribution { 0.25f, 0.1f }(rand), 0.05f);

        wave_pos = 0;
        wave_accum = 0.0f;
        wave_after = max(normal_distribution { 1.0f, 0.3f }(rand), 0.1f);

        for (auto& [index, spin] : chars) {
            if (bernoulli_distribution { 7.0f / 8.0f }(rand)) {
                spin = bernoulli_distribution { 1.0f / 20.0f }(rand);
                index = glyph_indices[glyph_indices_distr(rand)];
            } else {
                index = glyph_indices[0];
            }
        }
    }

    array<float, 3> position;
    array<float, 3> velocity;

    bool erasing;

    unsigned int feeder_char;
    float feeder_pos;
    float feeder_speed;

    array<pair<unsigned int, bool>, grid_size> chars;

    float spin_accum;
    float spin_after;

    unsigned int wave_pos;
    float wave_accum;
    float wave_after;
};

vector<strip> strips;

//----------------------------------------------------------------------------

void init(const options& opts, const array<unsigned int, 2>& window_size)
{
    if (auto status { glewInit() }; status != GLEW_OK) {
        throw runtime_error(reinterpret_cast<const char*>(glewGetErrorString(status)));
    }

    if (!GLEW_VERSION_3_0) {
        throw runtime_error("OpenGL 3.0 required");
    }

    // Build GLSL program

    const auto aspect { static_cast<float>(window_size[0]) / static_cast<float>(window_size[1]) };

    prog = make_unique<program>();
    prog->bind_attrib_location(position_attrib, "position");
    prog->bind_attrib_location(tex_coord_attrib, "texCoord");
    prog->bind_frag_data_location(frag_color_attrib, "fragColor");
    prog->link(vertex_shader { vertex_src }, fragment_shader { frag_src });
    prog->use();

    prog->set_uniform(prog->uniform_location("projection"), perspective(radians(80.0f), aspect, 0.1f, 100.0f));
    prog->set_uniform(prog->uniform_location("view"), translate({ 0.0f, 0.0f, -25.0f }));

    prog->set_uniform(prog->uniform_location("charColor"), opts.char_color);
    prog->set_uniform(prog->uniform_location("enableFog"), opts.enable_fog);
    prog->set_uniform(prog->uniform_location("enableWaves"), opts.enable_waves);
    prog->set_uniform(prog->uniform_location("feederColor"), opts.feeder_color);
    prog->set_uniform(prog->uniform_location("fontTex"), static_cast<GLint>(0));

    model_uniform = prog->uniform_location("model");

    char_pos_uniform = prog->uniform_location("charPos");
    feeder_pos_uniform = prog->uniform_location("feederPos");
    is_erasing_uniform = prog->uniform_location("isErasing");
    is_feeder_uniform = prog->uniform_location("isFeeder");
    wave_pos_uniform = prog->uniform_location("wavePos");

    // Make vertex buffer

    {
        const auto [v, i] = font.buffer_data();

        // Create and fill buffer

        font_vertex_buf = make_unique<vertex_buffer>(v.data(), v.size() * sizeof(float));
        font_vertex_buf->bind();

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

        glyph_indices.push_back(i.at(' '));
        switch (opts.mode) {
        case options::binary:
            for (const char16_t c : string_view { "01" }) {
                glyph_indices.push_back(i.at(c));
            }
            break;
        case options::dna:
            for (const char16_t c : string_view { "ACGT" }) {
                glyph_indices.push_back(i.at(c));
            }
            break;
        case options::decimal:
            for (const char16_t c : string_view { "0123456789" }) {
                glyph_indices.push_back(i.at(c));
            }
            break;
        case options::hexadecimal:
            for (const char16_t c : string_view { "0123456789ABCDEF" }) {
                glyph_indices.push_back(i.at(c));
            }
            break;
        case options::matrix:
            for (const auto [c, j] : i) {
                if (c == '1' || c == '7' || c == '0' || c == 'Z' || c == ':' || c == '.' || c == '"' || c == '=' || c == '*' || c == '+' || c == '-' || c == 166 || c == '|' || c > 256) {
                    glyph_indices.push_back(j);
                }
            }
            break;
        };
    }

    // Load font texture

    font_tex = make_unique<texture>(GL_R8, font_image_size[0], font_image_size[1], GL_RED, GL_UNSIGNED_BYTE, font_image_data);

    // Other GL init

    glViewport(0, 0, window_size[0], window_size[1]);
    glClearColor(opts.clear_color[0], opts.clear_color[1], opts.clear_color[2], 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create strips

    strips.resize(100);
    for (strip& s : strips) {
        s.reset();
    }
}

void update(const duration<double>& dt)
{
    // TODO: parallel?
    for (strip& s : strips) {
        s.update(dt);
    }
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    sort(begin(strips), end(strips));
    for (const strip& s : strips) {
        s.render();
    }
}

//----------------------------------------------------------------------------

volatile sig_atomic_t paused { 0 };

void signal_handler(int sig) noexcept
{
    if (sig == SIGUSR1) {
        paused = true;
    } else if (sig == SIGUSR2) {
        paused = false;
    }
}

} // namespace

int main(int argc, char* argv[])
{
    milliseconds frame_interval { 1'000 / 30 };
    unique_ptr<render_context> ctx;

    {
        const auto opts { parse_options(argc, argv) };
        frame_interval = milliseconds { 1'000 / opts.frame_rate };
        ctx = make_unique<render_context>();
        init(opts, ctx->window_size());
    }

    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    auto frame_tick { steady_clock::now() };

    while (true) {
        this_thread::sleep_until(frame_tick += frame_interval);
        if (!paused && !ctx->window_obscured()) {
            update(frame_interval);
            render();
        }
        ctx->swap_buffers();
    }

    return 0;
}
