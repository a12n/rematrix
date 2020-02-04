#include <chrono>
#include <functional>
#include <memory>
#include <thread>

#include <X11/Xlib.h>

#include <GL/glew.h>
#include <GL/glx.h>

namespace this_thread = std::this_thread;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::function;
using std::make_pair;
using std::pair;
using std::runtime_error;
using std::unique_ptr;

namespace {

// Creates and activates GLX context for rendering on the root window.
class rendering_context
{
public:
    rendering_context() :
        display_(open_display_()),
        window_(RootWindow(display_.get(), screen_())),
        context_(create_context_(choose_visual_(screen_())))
    {
        if (glXMakeCurrent(display_.get(), window_, context_.get()) != True) {
            throw runtime_error("couldn't activate rendering context");
        }
    }

    void
    swap_buffers()
    {
        glXSwapBuffers(display_.get(), window_);
    }

    pair<unsigned int, unsigned int>
    window_size() const
    {
        Window root;
        int x, y;
        unsigned int width, height;
        unsigned int border_width, depth;

        if (! XGetGeometry(display_.get(), window_,
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

private:
    typedef unique_ptr<Display, decltype(&XCloseDisplay)> display_ptr;
    typedef unique_ptr<XVisualInfo, decltype(&XFree)> visual_ptr;
    typedef unique_ptr<__GLXcontextRec, function<void(GLXContext)>> context_ptr; // XXX

    static display_ptr
    open_display_()
    {
        display_ptr ans(XOpenDisplay(nullptr), XCloseDisplay);

        if (! ans) {
            throw runtime_error("couldn't open display");
        }

        return ans;
    }

    int
    screen_() const
    {
        return DefaultScreen(display_.get());
    }

    visual_ptr
    choose_visual_(int screen) const
    {
        int attrs[] = {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            None
        };

        visual_ptr ans(glXChooseVisual(display_.get(), screen, attrs), XFree);

        if (! ans) {
            throw runtime_error("couldn't choose visual for rendering context");
        }

        return ans;
    }

    context_ptr
    create_context_(visual_ptr visual) const
    {
        auto context_deleter =
            [this] (GLXContext c) -> void {
                glXDestroyContext(this->display_.get(), c);
            };
        context_ptr ans(glXCreateContext(display_.get(), visual.get(), nullptr, True), context_deleter);

        if (! ans) {
            throw runtime_error("couldn't create rendering context");
        }

        return ans;
    }

    display_ptr display_;
    Window window_;
    context_ptr context_;
};

//----------------------------------------------------------------------------

class frame_rate_limit
{
public:
    explicit
    frame_rate_limit(unsigned int fps) :
        dt_(1000 / fps),
        t_(steady_clock::now())
    {
    }

    void
    wait()
    {
        this_thread::sleep_until(t_ += dt_);
    }

private:
    const milliseconds dt_;
    steady_clock::time_point t_;
};

//----------------------------------------------------------------------------

void
init(const pair<unsigned int, unsigned int>& window_size)
{
    glViewport(0, 0, window_size.first, window_size.second);
}

void
render()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace

int
main()
{
    rendering_context ctx;
    frame_rate_limit fps(10);

    init(ctx.window_size());
    while (true) {
        render();
        ctx.swap_buffers();
        fps.wait();
    }

    return 0;
}
