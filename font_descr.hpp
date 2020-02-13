#ifndef REMATRIX_FONT_HPP
#define REMATRIX_FONT_HPP

#include "decl.hpp"

namespace rematrix {

template <typename T>
struct glyph_descr
{
    // Texture coordinates of the glyph in GL_TRIANGLE_STRIP order.
    constexpr array<T, 8>
    coords() const
    {
        const auto [x, y] = pos;
        const auto [w, h] = size;
        return {{ x, y + h,
                  x + w, y + h,
                  x, y,
                  x + w, y }};
    }

    const array<T, 2> pos;
    const array<T, 2> size;
};

struct font_descr
{
    array<float, 8>
    tex_coords(char c) const
    {
        return glyphs.at(c).coords();
    }

    pair<vector<float>, unordered_map<char, uintptr_t>>
    tex_coords_data() const
    {
        vector<float> coords;
        unordered_map<char, uintptr_t> offsets;

        for (const auto [c, g] : glyphs) {
            const auto off = coords.size() * sizeof(float);
            offsets.insert(make_pair(c, off));
            const auto uv = g.coords();
            coords.insert(end(coords), begin(uv), end(uv));
        }

        return make_pair(coords, offsets);
    }

    const unordered_map<char, glyph_descr<float>> glyphs;
};

} // namespace rematrix

#endif  // REMATRIX_FONT_HPP
