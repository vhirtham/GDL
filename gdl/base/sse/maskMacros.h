#pragma once


//! @brief Creates a mask for blend intrinsics
//! @param a: First result value is taken from: 0 = first register, 1 = second register
//! @param b: Second result value is taken from: 0 = first register, 1 = second register
#define BLEND_2_MASK(a, b) (((b) << 1) | (a))

//! @brief Creates a mask for blend intrinsics
//! @param a: First result value is taken from: 0 = first register, 1 = second register
//! @param b: Second result value is taken from: 0 = first register, 1 = second register
//! @param c: Third result value is taken from: 0 = first register, 1 = second register
//! @param d: Fourth result value is taken from: 0 = first register, 1 = second register
#define BLEND_4_MASK(a, b, c, d) (((d) << 3) | ((c) << 2) | ((b) << 1) | (a))

//! @brief Creates a mask for blend intrinsics
//! @param a: First result value is taken from: 0 = first register, 1 = second register
//! @param b: Second result value is taken from: 0 = first register, 1 = second register
//! @param c: Third result value is taken from: 0 = first register, 1 = second register
//! @param d: Fourth result value is taken from: 0 = first register, 1 = second register
//! @param e: Fifth result value is taken from: 0 = first register, 1 = second register
//! @param f: Sixth result value is taken from: 0 = first register, 1 = second register
//! @param g: Seventh result value is taken from: 0 = first register, 1 = second register
//! @param h: Eighth result value is taken from: 0 = first register, 1 = second register
#define BLEND_8_MASK(a, b, c, d, e, f, g, h)                                                                           \
    (((h) << 7) | ((g) << 6) | ((f) << 5) | ((e) << 4) | ((d) << 3) | ((c) << 2) | ((b) << 1) | (a))

//! @brief Creates a mask for permute intrinsics
//! @param a: Index of a value that is used as first result value
//! @param b: Index of a value that is used as second result value
#define PERMUTE_2_MASK(a, b) (((b) << 1) | (a))

//! @brief Creates a mask for permute intrinsics
//! @param a: Index of a value that is used as first result value
//! @param b: Index of a value that is used as second result value
//! @param c: Index of a value that is used as third result value
//! @param d: Index of a value that is used as fourth result value
#define PERMUTE_4_MASK(a, b, c, d) (((d) << 6) | ((c) << 4) | ((b) << 2) | (a))

//! @brief Creates a mask for _mm256_permute_pd
//! @param a: First result value of first lane is: 0 = first value, 1 = second value of source registers first lane
//! @param b: Second result value of first lane is: 0 = first value, 1 = second value of source registers first lane
//! @param c: First result value of second lane is: 0 = first value, 1 = second value of source registers second lane
//! @param d: Second result value of second lane is: 0 = first value, 1 = second value of source registers second lane
#define PERMUTE256_PD_MASK(a, b, c, d) (((d) << 3) | ((c) << 2) | ((b) << 1) | (a))
