#ifndef REMATRIX_MATRIX_HPP
#define REMATRIX_MATRIX_HPP

#include "decl.hpp"

namespace rematrix {

using vector4 = array<float, 4>;
using matrix4 = array<vector4, 4>;

inline const matrix4 identity_matrix{
    { {1.0, 0.0, 0.0, 0.0},
      {0.0, 1.0, 0.0, 0.0},
      {0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0} }
};

} // namespace rematrix

#endif  // REMATRIX_MATRIX_HPP
