#define BOOST_TEST_MODULE mat4 test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "GDLTypedefs.h"
#include "math/mat4SIMD.inl"
#include "math/mat4Single.inl"

#include <cmath>

using namespace GDL;

template <typename T>
bool checkClose(T a, T b, F32 tolerance = 1e-6)
{
    for (U32 i = 0; i < 4; ++i)
        for (U32 j = 0; j < 4; ++j)
            if (std::fabs(a(i, j) - b(i, j)) > tolerance)
                return false;
    return true;
}

BOOST_AUTO_TEST_CASE(Multiplication_Single)
{
    mat4Single<F32> A(std::array<F32, 16>{{0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}});
    mat4Single<F32> B(std::array<F32, 16>{{8, 11, 6, 2, 5, 4, 9, 3, 3, 7, 5, 2, 6, 8, 2, 6}});

    auto C = A * B;
    mat4Single<F32> expC(
            std::array<F32, 16>{{29, 137, 245, 353, 31, 115, 199, 283, 23, 91, 159, 227, 30, 118, 206, 294}});
    BOOST_CHECK(checkClose(C, expC));

    auto D = B * A;
    mat4Single<F32> expD(
            std::array<F32, 16>{{116, 168, 100, 100, 138, 198, 122, 113, 160, 228, 144, 126, 182, 258, 166, 139}});
    BOOST_CHECK(checkClose(D, expD));
}


BOOST_AUTO_TEST_CASE(Multiplication_SIMD)
{
    mat4SIMD A(__m128{0, 4, 8, 12}, __m128{1, 5, 9, 13}, __m128{2, 6, 10, 14}, __m128{3, 7, 11, 15});
    mat4SIMD B(__m128{8, 11, 6, 2}, __m128{5, 4, 9, 3}, __m128{3, 7, 5, 2}, __m128{6, 8, 2, 6});

    auto C = A * B;
    mat4SIMD expC(__m128{29, 137, 245, 353}, __m128{31, 115, 199, 283}, __m128{23, 91, 159, 227},
                  __m128{30, 118, 206, 294});
    BOOST_CHECK(checkClose(C, expC));

    auto D = B * A;
    mat4SIMD expD(__m128{116, 168, 100, 100}, __m128{138, 198, 122, 113}, __m128{160, 228, 144, 126},
                  __m128{182, 258, 166, 139});
    BOOST_CHECK(checkClose(D, expD));
}
