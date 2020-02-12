#ifndef REMATRIX_DECL_HPP
#define REMATRIX_DECL_HPP

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <string_view>
#include <thread>
#include <unordered_map>

namespace rematrix {

namespace this_thread = std::this_thread;
using std::array;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::steady_clock;
using std::function;
using std::make_pair;
using std::make_unique;
using std::pair;
using std::remove_pointer_t;
using std::runtime_error;
using std::string;
using std::string_view;
using std::unique_ptr;
using std::unordered_map;

class rendering_context;
struct buffer;
struct element_buffer;
struct fragment_shader;
struct program;
struct shader;
struct vertex_buffer;
struct vertex_shader;

} // namespace rematrix

#endif  // REMATRIX_DECL_HPP
