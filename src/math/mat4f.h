#include "GDLTypedefs.h"

#include <x86intrin.h>

#ifndef NDEBUG
#include <iostream>
#endif

namespace GDL
{

bool is_aligned(const volatile void* p, size_t n)
{
    return reinterpret_cast<std::uintptr_t>(p) % n == 0;
}


class __attribute__((aligned(16))) mat4f
{
    __attribute__((aligned(16))) __m128 mCol0;
    __attribute__((aligned(16))) __m128 mCol1;
    __attribute__((aligned(16))) __m128 mCol2;
    __attribute__((aligned(16))) __m128 mCol3;

public:
    mat4f();
    mat4f(__m128 col0, __m128 col1, __m128 col2, __m128 col3);

    inline mat4f operator*(const mat4f& pOther_cr) const;
    inline F32 operator()(const U32 row, const U32 col) const;

private:
#ifndef NDEBUG
    friend std::ostream& operator<<(std::ostream&, mat4f const&);
#endif
};

} // namespace GDL
