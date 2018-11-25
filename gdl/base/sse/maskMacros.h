#pragma once


#define GDL_SHUFFLE2(a, b) (((b) << 1) | (a))
#define GDL_SHUFFLE4(a, b, c, d) (((d) << 6) | ((c) << 4) | ((b) << 2) | (a))
