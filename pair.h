#ifndef PAIR_H
#define PAIR_H

#include <cstdlib>

// Represents a coordinate pair where neither coordinate is negative
struct Pair
{
	size_t x;
	size_t y;

	Pair() : x(0), y(0) {}
	Pair(size_t x, size_t y) : x(x), y(y) {}
};

#endif // PAIR_H
