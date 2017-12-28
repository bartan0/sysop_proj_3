#include <iostream>

#ifndef DEBUG
#define DEBUG
#endif

#include "debug.hpp"

template <typename T>
void debug_log_arr(T *xs, size_t l) {
	std::cout << "[";
	if (l > 0)
		std::cout << xs[0];
	for (size_t i = 1; i < l; i++)
		std::cout << ", " << xs[i];
	std::cout << "]\n";
}
