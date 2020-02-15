#ifndef REMATRIX_DECL_HPP
#define REMATRIX_DECL_HPP

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace rematrix {

namespace this_thread = std::this_thread;
using std::array;
using std::begin;
using std::bernoulli_distribution;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::end;
using std::function;
using std::make_pair;
using std::make_unique;
using std::max;
using std::minstd_rand;
using std::normal_distribution;
using std::pair;
using std::remove_pointer_t;
using std::runtime_error;
using std::sort;
using std::string;
using std::string_view;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

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
