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
