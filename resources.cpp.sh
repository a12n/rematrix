#!/bin/sh

cat<<EOF
#include "resources.hpp"

namespace rematrix {

const char vertex_src[] = R"(
$(cat data/vertex.glsl)
)";

const char fragment_src[] = R"(
$(cat data/fragment.glsl)
)";

const array<unsigned short, 2> font_image_size{$(identify -format '%w, %h' data/font.png)};
const unsigned char font_image_data[]{
$(convert data/font.png -depth 8 gray:- | od -v -A n -t u1 -w1 | sed 's/^ *//;s/$/,/')
};

const font_descr font{
$(./cmds/font_descr2 < data/font.fnt)
};

} // namespace rematrix
EOF
