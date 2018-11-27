#pragma once


//! @brief Creates a mask for shuffle intrinsics
//! @param a: Index of a value of the first source register that is used as first result value
//! @param b: Index of a value of the second source register that is used as second result value
#define SHUFFLE_MASK_2(a, b) (((b) << 1) | (a))

//! @brief Creates a mask for shuffle intrinsics
//! @param a: Index of a value of the first source register that is used as first result value
//! @param b: Index of a value of the first source register that is used as second result value
//! @param c: Index of a value of the second source register that is used as third result value
//! @param d: Index of a value of the second source register that is used as fourth result value
#define SHUFFLE_MASK_4(a, b, c, d) (((d) << 6) | ((c) << 4) | ((b) << 2) | (a))

//! @brief Creates a mask for blend intrinsics
//! @param a: First result value is taken from: 0 = first register, 1 = second register
//! @param b: Second result value is taken from: 0 = first register, 1 = second register
#define BLEND_MASK_2(a, b) (((b) << 1) | (a))

//! @brief Creates a mask for blend intrinsics
//! @param a: First result value is taken from: 0 = first register, 1 = second register
//! @param b: Second result value is taken from: 0 = first register, 1 = second register
//! @param c: Third result value is taken from: 0 = first register, 1 = second register
//! @param d: Fourth result value is taken from: 0 = first register, 1 = second register
#define BLEND_MASK_4(a, b, c, d) (((d) << 3) | ((c) << 2) | ((b) << 1) | (a))

//! @brief Creates a mask for blend intrinsics
//! @param a: First result value is taken from: 0 = first register, 1 = second register
//! @param b: Second result value is taken from: 0 = first register, 1 = second register
//! @param c: Third result value is taken from: 0 = first register, 1 = second register
//! @param d: Fourth result value is taken from: 0 = first register, 1 = second register
//! @param e: Fifth result value is taken from: 0 = first register, 1 = second register
//! @param f: Sixth result value is taken from: 0 = first register, 1 = second register
//! @param g: Seventh result value is taken from: 0 = first register, 1 = second register
//! @param h: Eighth result value is taken from: 0 = first register, 1 = second register
#define BLEND_MASK_8(a, b, c, d, e, f, g, h)                                                                           \
    (((h) << 7) | ((g) << 6) | ((f) << 5) | ((e) << 4) | ((d) << 3) | ((c) << 2) | ((b) << 1) | (a))

//! @brief Creates a mask for the _mm256_permute2f128 intrinsics
//! @param r0: Source register index for the lower 128bits of the result (0 = first / 1 = second of passed registers)
//! @param v0: Value index for the lower 128bits of the result (0 = higher / 1 = lower 128 bits of source register)
//! @param r1: Source register index for the higher 128bits of the result (0 = first / 1 = second of passed registers)
//! @param v1: Value index for the higher 128bits of the result (0 = higher / 1 = lower 128 bits of source register)
#define PERMUTE_MASK_2F128(r0, v0, r1, v1) (((r1) << 5) | ((v1) << 4) | ((r0) << 1) | (v0))
