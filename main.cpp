#include "rendering_context.hpp"

using namespace rematrix;

int
main()
{
    rendering_context ctx;

    while (true) {
        ctx.swap_buffers();
    }

    return 0;
}
