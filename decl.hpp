#ifndef REMATRIX_DECL_HPP
#define REMATRIX_DECL_HPP

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <string_view>
#include <thread>

namespace rematrix {

namespace this_thread = std::this_thread;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::steady_clock;
using std::function;
using std::make_pair;
using std::pair;
using std::remove_pointer_t;
using std::runtime_error;
using std::string_view;
using std::unique_ptr;

} // namespace rematrix

#endif  // REMATRIX_DECL_HPP
