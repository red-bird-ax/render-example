#pragma once

#include <cstdio>
#include <cstdint>
#include <cstdlib>

using ID = uint32_t;

#define ASSERT(x, msg) \
{ \
	auto r = (x); \
	if (!r) \
	{ \
		printf("Assertion failed: %s\n", msg); \
		std::exit(-1); \
	} \
}