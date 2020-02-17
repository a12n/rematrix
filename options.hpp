#ifndef REMATRIX_OPTIONS_HPP
#define REMATRIX_OPTIONS_HPP

#include "matrix.hpp"

namespace rematrix {

struct options
{
    enum mode {
        binary,
        decimal,
        dna,
        hexadecimal,
        matrix,
    };

    bool enable_fog{false};
    bool enable_waves{true};
    mode mode{matrix};
    unsigned int frame_rate{15};
    vec3 char_color{0x45 / 255.0f, 0x85 / 255.0f, 0x88 / 255.0f};
    vec3 clear_color{0x28 / 255.0f, 0x28 / 255.0f, 0x28 / 255.0f};
    vec3 feeder_color{0x83 / 255.0f, 0xa5 / 255.0f, 0x98 / 255.0f};
};

options
parse_options(int argc, char* argv[]);

} // namespace rematrix

#endif  // REMATRIX_OPTIONS_HPP
