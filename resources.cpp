#include "resources.hpp"

namespace rematrix {

const char vertex_src[] =
#include "vertex_src.inc"
    ;

const char frag_src[] =
#include "frag_src.inc"
    ;

const char blur_vertex_src[] =
#include "blur_vertex_src.inc"
    ;

const char blur_frag_src[] =
#include "blur_frag_src.inc"
    ;

const array<unsigned short, 2> font_image_size{
#include "font_image_size.inc"
};
const unsigned char font_image_data[]{
#include "font_image_data.inc"
};

const font_descr font{
#include "font.inc"
};

} // namespace rematrix
