#ifndef REMATRIX_FONT_HPP
#define REMATRIX_FONT_HPP

#include "decl.hpp"

namespace rematrix {

struct glyph_descr
{
    const array<float, 2> pos;
    const array<float, 2> size;
};

struct font_descr
{
    array<float, 8>
    tex_coords(char c) const
    {
        return tex_coords(glyphs.at(c));
    }

    array<float, 8>
    tex_coords(const glyph_descr& g) const
    {
        auto [x, y] = g.pos;
        auto [w, h] = g.size;
        return {{ x, y + h,
                  x + w, y + h,
                  x, y,
                  x + w, y }};
    }

    const unordered_map<char, glyph_descr> glyphs;
};

} // namespace rematrix

#endif  // REMATRIX_FONT_HPP
