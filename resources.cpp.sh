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

const unsigned short font_image_width{$(identify -format '%w' data/font.png)};
const unsigned short font_image_height{$(identify -format '%h' data/font.png)};
const unsigned char font_image_data[]{
$(convert data/font.png -depth 8 gray:- | od -v -A n -t u1 -w1 | sed 's/^ *//;s/$/,/')
};

const font_descr font{ {
$(jq -r '. as $top | .characters | to_entries[] | "{ '\''\(.key)'\'', {{\(.value.x / $top.width), \(.value.y / $top.height)}, {\(.value.width / $top.width), \(.value.height / $top.height)}} },"' data/font.json)
} };

} // namespace rematrix
EOF
