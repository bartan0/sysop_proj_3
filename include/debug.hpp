#ifndef _DEBUG_HPP
#define _DEBUG_HPP

#ifdef DEBUG

#include <cstddef>
#include <cstdio>

#define debug_log(msg) std::printf(msg)
#define debug_logf(format, ...) std::printf(format, __VA_ARGS__)

template <typename T>
void debug_log_arr(T *xs, size_t l);

#else // DEBUG

#define debug_log(...) void()
#define debug_logf(...) void()
#define debug_log_arr(...) void()

#endif // DEBUG

#endif // _DEBUG_HPP
