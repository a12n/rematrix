#ifndef REMATRIX_RESOURCES_HPP
#define REMATRIX_RESOURCES_HPP

#include "font_descr.hpp"

namespace rematrix {

extern const char vertex_src[];
extern const char frag_src[];

extern const char blur_vertex_src[];
extern const char blur_frag_src[];

extern const array<unsigned short, 2> font_image_size;
extern const unsigned char font_image_data[];

extern const font_descr font;

} // namespace rematrix

#endif  // REMATRIX_RESOURCES_HPP
