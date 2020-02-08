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

    while (true) {
        render(duration<float>{});
        ctx.swap_buffers();
    }

    return 0;
}
