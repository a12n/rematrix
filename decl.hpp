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
using std::cbegin;
using std::cend;
using std::end;
using std::function;
using std::initializer_list;
using std::make_pair;
using std::make_unique;
using std::max;
using std::minstd_rand;
using std::normal_distribution;
using std::pair;
using std::remove_pointer_t;
using std::runtime_error;
using std::sort;
using std::stoul;
using std::string;
using std::string_view;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::unique_ptr;
using std::unordered_map;
using std::vector;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

class render_context;
struct buffer;
struct element_buffer;
struct font_descr;
struct fragment_shader;
struct glyph_descr;
struct options;
struct program;
struct shader;
struct texture;
struct vertex_buffer;
struct vertex_shader;

} // namespace rematrix

#endif // REMATRIX_DECL_HPP
