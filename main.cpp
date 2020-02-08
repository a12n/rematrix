#include <GL/glew.h>

#include "rendering_context.hpp"

using namespace rematrix;

namespace {

void
init(const pair<unsigned int, unsigned int>& window_size)
{
    if (auto status{glewInit()}; status != GLEW_OK) {
        throw runtime_error(reinterpret_cast<const char*>(glewGetErrorString(status)));
    }

    if (! GLEW_VERSION_3_0) {
        throw runtime_error("OpenGL 3.0 required");
    }

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
