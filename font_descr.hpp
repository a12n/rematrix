#ifndef REMATRIX_FONT_HPP
#define REMATRIX_FONT_HPP

#include "decl.hpp"

namespace rematrix {

struct glyph_descr
{
    // Image coordinates of the glyph in triangle strip order.
    constexpr array<unsigned short, 8>
    coords() const
    {
        const auto [x, y] = pos;
        const auto [w, h] = size;
        return {{ x, static_cast<unsigned short>(y + h),
                  static_cast<unsigned short>(x + w), static_cast<unsigned short>(y + h),
                  x, y,
                  static_cast<unsigned short>(x + w), y }};
    }

    // Normalized texture coordinates.
    constexpr array<float, 8>
    coords(array<unsigned short, 2> image_size) const
    {
        const auto [u1, w1, u2, w2, u3, w3, u4, w4] = coords();
        const float w = static_cast<float>(image_size[0]);
        const float h = static_cast<float>(image_size[1]);
        return {{ static_cast<float>(u1) / w,
                  static_cast<float>(w1) / h,
                  static_cast<float>(u2) / w,
                  static_cast<float>(w2) / h,
                  static_cast<float>(u3) / w,
                  static_cast<float>(w3) / h,
                  static_cast<float>(u4) / w,
                  static_cast<float>(w4) / h }};
    }

    const array<unsigned short, 2> pos{0, 0};
    const array<unsigned short, 2> size{0, 0};
    const array<short, 2> offset{0, 0};
    const unsigned short advance{0};
};

struct font_descr
{
    array<float, 8>
    coords(char c) const
    {
        return glyphs.at(c).coords(image_size);
    }

    pair<vector<float>, unordered_map<char, uintptr_t>>
    coords_data() const
    {
        vector<float> coords;
        unordered_map<char, uintptr_t> offsets;

        for (const auto [c, g] : glyphs) {
            const auto off = coords.size() * sizeof(float);
            offsets.insert(make_pair(c, off));
            const auto uv = g.coords(image_size);
            coords.insert(end(coords), begin(uv), end(uv));
        }

        return make_pair(coords, offsets);
    }

    const unsigned short font_size{0};
    const array<unsigned short, 2> image_size{0, 0};
    const array<unsigned short, 4> padding{0, 0, 0, 0};
    const array<unsigned short, 2> spacing{0, 0};
    const unsigned short line_height{0};
    const unsigned short base{0};
    const unordered_map<char, glyph_descr> glyphs;
};

} // namespace rematrix

#endif  // REMATRIX_FONT_HPP
