#ifndef REMATRIX_FONT_HPP
#define REMATRIX_FONT_HPP

#include "decl.hpp"

namespace rematrix {

struct glyph_descr
{
    const array<double, 2> pos;
    const array<double, 2> size;
};

struct font_descr
{
    const unordered_map<char, glyph_descr> glyphs;
};

} // namespace rematrix

#endif  // REMATRIX_FONT_HPP
