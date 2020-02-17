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
        return {{ x, y,
                  static_cast<unsigned short>(x + w), y,
                  x, static_cast<unsigned short>(y + h),
                  static_cast<unsigned short>(x + w), static_cast<unsigned short>(y + h) }};
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
    position(const glyph_descr& g) const
    {
        const auto fs{static_cast<float>(font_size)};

        const auto x1{-g.advance / 2.0f + g.offset[0]};
        const auto y1{fs / 2.0f - g.offset[1]};
        const auto x2{x1 + g.size[0]};
        const auto y2{y1 - g.size[1]};

        return {{ x1 / fs, y1 / fs,
                  x2 / fs, y1 / fs,
                  x1 / fs, y2 / fs,
                  x2 / fs, y2 / fs }};
    }

    // Generate vertex buffer contents with interleaved vertex
    // position (2 floats) and texture coordinates (2 floats), for
    // GL_TRIANGLE_STRIP rendering. Returns mapping of characters to
    // vertex indices for glDrawArrays.
    pair<vector<float>, unordered_map<char16_t, unsigned int>>
    buffer_data() const
    {
        vector<float> ans;
        unordered_map<char16_t, unsigned int> indices;

        for (const auto [c, g] : glyphs) {
            const auto off = ans.size() / 4;
            indices.insert(make_pair(c, off));
            const auto [x1, y1, x2, y2, x3, y3, x4, y4] = position(g);
            const auto [u1, v1, u2, v2, u3, v3, u4, v4] = g.coords(image_size);
            const auto interleaved = {x1, y1, u1, v1,
                                      x2, y2, u2, v2,
                                      x3, y3, u3, v3,
                                      x4, y4, u4, v4};
            ans.insert(end(ans), begin(interleaved), end(interleaved));
        }

        return make_pair(ans, indices);
    }

    const unsigned short font_size{0};
    const array<unsigned short, 2> image_size{0, 0};
    const array<unsigned short, 4> padding{0, 0, 0, 0};
    const array<short, 2> spacing{0, 0};
    const unsigned short line_height{0};
    const unsigned short base{0};
    const unordered_map<char16_t, glyph_descr> glyphs;
};

} // namespace rematrix

#endif  // REMATRIX_FONT_HPP
