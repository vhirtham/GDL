#pragma once

// INFO: The whole purpose of this header is to avoid adding the lines to turn off the ignored-attributes gcc warning
// every time x86intrin.h needs to be included.

#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include <x86intrin.h>
