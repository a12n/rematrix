#ifndef REMATRIX_RENDER_CONTEXT_HPP
#define REMATRIX_RENDER_CONTEXT_HPP

#include <X11/Xlib.h>

#include <GL/glx.h>

#include "decl.hpp"

namespace rematrix {

class render_context {
public:
    render_context();

    void swap_buffers();

    array<unsigned int, 2> window_size() const;

    bool window_obscured() const;

private:
    using display_ptr = unique_ptr<Display, decltype(&XCloseDisplay)>;
    using context_ptr = unique_ptr<remove_pointer_t<GLXContext>, function<void(GLXContext)>>;

    static display_ptr open_display();

    GLXFBConfig choose_fb_config(int screen) const;

    context_ptr create_context(GLXFBConfig config) const;

    void update_visibility();

    const display_ptr display;
    const Window window;
    const context_ptr context;
    bool obscured { false };
};

} // namespace rematrix

#endif // REMATRIX_RENDER_CONTEXT_HPP
