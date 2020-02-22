#include "rendering_context.hpp"

namespace rematrix {

rendering_context::rendering_context() :
    display{open_display()},
    window{XRootWindow(display.get(), XDefaultScreen(display.get()))},
    context{create_context(choose_fb_config(XDefaultScreen(display.get())))},
    obscured{false}
{
    if (! glXMakeCurrent(display.get(), window, context.get())) {
        throw runtime_error("couldn't activate rendering context");
    }
    // XSelectInput(display.get(), window, VisibilityChangeMask);
    // XSync(display.get(), False);
}

void
rendering_context::swap_buffers()
{
    glXSwapBuffers(display.get(), window);
    // update_visibility();
}

array<unsigned int, 2>
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

    return {{width, height}};
}

bool
rendering_context::window_obscured() const
{
    // const_cast<rendering_context*>(this)->update_visibility();
    return obscured;
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

void
rendering_context::update_visibility()
{
    while (XPending(display.get()) > 0) {
        XEvent ev;
        XNextEvent(display.get(), &ev);
        switch (ev.type) {
        case VisibilityNotify :
            obscured = (ev.xvisibility.state == VisibilityFullyObscured);
            break;
        default :
            break;
        };
    }
}

} // namespace rematrix
