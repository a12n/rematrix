#include <iostream>

#include <unistd.h>

#include "decl.hpp"
#include "options.hpp"

namespace rematrix {

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
