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

} // namespace rematrix
EOF
