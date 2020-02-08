#ifndef REMATRIX_RENDERING_CONTEXT_HPP
#define REMATRIX_RENDERING_CONTEXT_HPP

#include <X11/Xlib.h>

#include <GL/glx.h>

#include "decl.hpp"

namespace rematrix {

class rendering_context
{
public:
    rendering_context();

    void
    swap_buffers();

    pair<unsigned int, unsigned int>
    window_size() const;

private:
    typedef unique_ptr<Display, decltype(&XCloseDisplay)> display_ptr;
    typedef unique_ptr<remove_pointer_t<GLXContext>, function<void(GLXContext)>> context_ptr;

    static display_ptr
    open_display();

    GLXFBConfig
    choose_fb_config(int screen) const;

    context_ptr
    create_context(GLXFBConfig config) const;

    const display_ptr display;
    const Window window;
    const context_ptr context;
};

} // namespace rematrix

#endif  // REMATRIX_RENDERING_CONTEXT_HPP
