#ifndef REMATRIX_MATRIX_HPP
#define REMATRIX_MATRIX_HPP

#include <cmath>

#include "decl.hpp"

namespace rematrix {

using vec3 = array<float, 3>;
using vec4 = array<float, 4>;
using mat4 = array<vec4, 4>;

inline const mat4 identity{
    { { 1.0f, 0.0f, 0.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f, 1.0f } }
};

constexpr float
radians(float degrees)
{
    return degrees * M_PI / 180.0f;
}

constexpr mat4
translate(const vec3& v)
{
    return {
        { { 1.0f, 0.0f, 0.0f, 0.0f},
          { 0.0f, 1.0f, 0.0f, 0.0f},
          { 0.0f, 0.0f, 1.0f, 0.0f},
          { v[0], v[1], v[2], 1.0f} }
    };
}

inline mat4
perspective(float fovy, float aspect, float near, float far)
{
    const auto tan_half_fovy{tanf(fovy / 2.0f)};
    return {
        { {1.0f / (aspect * tan_half_fovy), 0.0f, 0.0f, 0.0f},
          {0.0f, 1.0f / tan_half_fovy, 0.0f, 0.0f},
          {0.0f, 0.0f, -(far + near) / (far - near), -1.0f},
          {0.0f, 0.0f, -(2.0f * far * near) / (far - near), 1.0f} }
    };
}

} // namespace rematrix

#endif  // REMATRIX_MATRIX_HPP
