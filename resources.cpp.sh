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

const font_descr font{ {
$(jq -r '. as $top | .characters | to_entries[] | "{ '\''\(.key)'\'', {{\(.value.x / $top.width), \(.value.y / $top.height)}, {\(.value.width / $top.width), \(.value.height / $top.height)}} },"' data/font.json)
} };

} // namespace rematrix
EOF
