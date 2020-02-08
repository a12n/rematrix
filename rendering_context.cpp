#include "rendering_context.hpp"

namespace rematrix {

rendering_context::rendering_context() :
    display{open_display()},
    window{XRootWindow(display.get(), XDefaultScreen(display.get()))},
    context{create_context(choose_fb_config(XDefaultScreen(display.get())))}
{
    if (! glXMakeCurrent(display.get(), window, context.get())) {
        throw runtime_error("couldn't activate rendering context");
    }
}

void
rendering_context::swap_buffers()
{
    glXSwapBuffers(display.get(), window);
}

pair<unsigned int, unsigned int>
rendering_context::window_size() const
{
    Window root;
    int x, y;
    unsigned int width, height;
    unsigned int border_width, depth;

    if (! XGetGeometry(display.get(), window,
                       &root,
                       &x, &y,
                       &width, &height,
                       &border_width,
                       &depth))
    {
        throw runtime_error("couldn't get window size");
    }

    return make_pair(width, height);
}

rendering_context::display_ptr
rendering_context::open_display()
{
    display_ptr ans{XOpenDisplay(nullptr), XCloseDisplay};

    if (! ans) {
        throw runtime_error("couldn't open display");
    }

    return ans;
}

GLXFBConfig
rendering_context::choose_fb_config(int screen) const
{
    const int attrs[] = {
        GLX_DOUBLEBUFFER, True,
        GLX_X_RENDERABLE, True,
        GLX_SAMPLE_BUFFERS, 1,
        GLX_SAMPLES, 4,
        None
    };

    int n{0};
    unique_ptr<GLXFBConfig, decltype(&XFree)> configs{glXChooseFBConfig(display.get(), screen, attrs, &n), XFree};
    if (! configs || n < 1) {
        throw runtime_error("couldn't choose frame buffer configuration");
    }

    return configs.get()[0];
}

rendering_context::context_ptr
rendering_context::create_context(GLXFBConfig config) const
{
    auto context_deleter =
        [this] (GLXContext c) -> void {
            glXDestroyContext(display.get(), c);
        };
    context_ptr ans{glXCreateNewContext(display.get(), config, GLX_RGBA_TYPE, nullptr, True), context_deleter};

    if (! ans) {
        throw runtime_error("couldn't create rendering context");
    }

    return ans;
}

} // namespace rematrix
