#pragma once


#define SHUFFLE_MASK_2(a, b) (((b) << 1) | (a))
#define SHUFFLE_MASK_4(a, b, c, d) (((d) << 6) | ((c) << 4) | ((b) << 2) | (a))

#define BLEND_MASK_2(a, b) (((b) << 1) | (a))
#define BLEND_MASK_4(a, b, c, d) (((d) << 3) | ((c) << 2) | ((b) << 1) | (a))
