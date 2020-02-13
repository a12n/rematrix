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

    pair<vector<float>, unordered_map<char, uintptr_t>>
    tex_coords_data() const
    {
        vector<float> coords;
        unordered_map<char, uintptr_t> offsets;

        for (const auto [c, g] : glyphs) {
            const auto off = coords.size() * sizeof(float);
            offsets.insert(make_pair(c, off));
            const auto uv = tex_coords(g);
            coords.insert(end(coords), begin(uv), end(uv));
        }

        return make_pair(coords, offsets);
    }

    const unordered_map<char, glyph_descr> glyphs;
};

} // namespace rematrix

#endif  // REMATRIX_FONT_HPP
