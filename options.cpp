#include <iostream>

#include <unistd.h>

#include "decl.hpp"
#include "options.hpp"

namespace rematrix {
namespace {

vec3
parse_color(string_view s)
{
    const auto err{runtime_error("invalid color")};

    if (s.size() != 7 || s[0] != '#') {
        throw err;
    }

    size_t n{0};

    // XXX: allows "#+12345" or "#-12345"
    const auto color{stoul(string(s.substr(1)), &n, 16)};

    if (n != 6 || color > 0xFFFFFF) {
        throw err;
    }

    return {
        ((color >> 16) & 0xFF) / 255.0f,
        ((color >> 8) & 0xFF) / 255.0f,
        (color & 0xFF) / 255.0f
    };
}

} // namespace

options
parse_options(int argc, char* argv[])
{
    using std::cerr;
    using std::endl;

    options ans;

    int opt{-1};
    while ((opt = getopt(argc, argv, "")) != -1) {
        switch (opt) {
        default :
            cerr << "Usage: " << argv[0]
                 << endl;
            exit(EXIT_FAILURE);
            break;
        }
    }

    return ans;
}

} // namespace rematrix
