#define BOOST_TEST_MODULE mat4f test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "GDLTypedefs.h"
#include "math/mat4f.inl"

#include <cmath>

using namespace GDL;

bool checkClose(mat4f a, mat4f b, F32 tolerance = 1e-6)
{
    for (U32 i = 0; i < 4; ++i)
        for (U32 j = 0; j < 4; ++j)
            if (std::fabs(a(i, j) - b(i, j)) > tolerance)
                return false;
    return true;
}

BOOST_AUTO_TEST_CASE(Multiplication)
{
    mat4f A(__m128{0, 4, 8, 12}, __m128{1, 5, 9, 13}, __m128{2, 6, 10, 14}, __m128{3, 7, 11, 15});
    mat4f B(__m128{8, 11, 6, 2}, __m128{5, 4, 9, 3}, __m128{3, 7, 5, 2}, __m128{6, 8, 2, 6});

    auto C = A * B;
    mat4f expC(__m128{29, 137, 245, 353}, __m128{31, 115, 199, 283}, __m128{23, 91, 159, 227},
               __m128{30, 118, 206, 294});
    BOOST_CHECK(checkClose(C, expC));

    auto D = B * A;
    mat4f expD(__m128{116, 168, 100, 100}, __m128{138, 198, 122, 113}, __m128{160, 228, 144, 126},
               __m128{182, 258, 166, 139});
    BOOST_CHECK(checkClose(D, expD));
}
