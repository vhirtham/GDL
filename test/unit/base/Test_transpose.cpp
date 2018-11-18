#include <boost/test/unit_test.hpp>

#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/transpose.h"


#include <array>



using namespace GDL;

BOOST_AUTO_TEST_CASE(Transpose2x2_128)
{
    alignas(alignmentBytes<__m128d>) std::array<__m128d, 2> a, b, expected;
    a[0] = _mm_setr_pd(1, 2);
    a[1] = _mm_setr_pd(3, 4);

    expected[0] = _mm_setr_pd(1, 3);
    expected[1] = _mm_setr_pd(2, 4);


    Transpose(a[0], a[1], b[0], b[1]);

    for (U32 i = 0; i < 2; ++i)
        for (U32 j = 0; j < 2; ++j)
            BOOST_CHECK(b[i][j] == expected[i][j]);
}



BOOST_AUTO_TEST_CASE(Transpose4x4_128)
{
    alignas(alignmentBytes<__m128>) std::array<__m128, 4> a, b, expected;

    a[0] = _mm_setr_ps(1, 2, 3, 4);
    a[1] = _mm_setr_ps(5, 6, 7, 8);
    a[2] = _mm_setr_ps(9, 10, 11, 12);
    a[3] = _mm_setr_ps(13, 14, 15, 16);

    expected[0] = _mm_setr_ps(1, 5, 9, 13);
    expected[1] = _mm_setr_ps(2, 6, 10, 14);
    expected[2] = _mm_setr_ps(3, 7, 11, 15);
    expected[3] = _mm_setr_ps(4, 8, 12, 16);


    Transpose(a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3]);

    for (U32 i = 0; i < 4; ++i)
        for (U32 j = 0; j < 4; ++j)
            BOOST_CHECK(b[i][j] == expected[i][j]);
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(Transpose4x4_256)
{
    alignas(alignmentBytes<__m256d>) std::array<__m256d, 4> a, b, expected;
    a[0] = _mm256_setr_pd(1, 2, 3, 4);
    a[1] = _mm256_setr_pd(5, 6, 7, 8);
    a[2] = _mm256_setr_pd(9, 10, 11, 12);
    a[3] = _mm256_setr_pd(13, 14, 15, 16);

    expected[0] = _mm256_setr_pd(1, 5, 9, 13);
    expected[1] = _mm256_setr_pd(2, 6, 10, 14);
    expected[2] = _mm256_setr_pd(3, 7, 11, 15);
    expected[3] = _mm256_setr_pd(4, 8, 12, 16);

    Transpose(a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3]);


    for (U32 i = 0; i < 4; ++i)
        for (U32 j = 0; j < 4; ++j)
            BOOST_CHECK(b[i][j] == expected[i][j]);
}



BOOST_AUTO_TEST_CASE(Transpose8x8_256)
{
    alignas(alignmentBytes<__m256>) std::array<__m256, 8> a, b, expected;
    a[0] = _mm256_setr_ps(1, 2, 3, 4, 5, 6, 7, 8);
    a[1] = _mm256_setr_ps(9, 10, 11, 12, 13, 14, 15, 16);
    a[2] = _mm256_setr_ps(17, 18, 19, 20, 21, 22, 23, 24);
    a[3] = _mm256_setr_ps(25, 26, 27, 28, 29, 30, 31, 32);
    a[4] = _mm256_setr_ps(33, 34, 35, 36, 37, 38, 39, 40);
    a[5] = _mm256_setr_ps(41, 42, 43, 44, 45, 46, 47, 48);
    a[6] = _mm256_setr_ps(49, 50, 51, 52, 53, 54, 55, 56);
    a[7] = _mm256_setr_ps(57, 58, 59, 60, 61, 62, 63, 64);


    expected[0] = _mm256_setr_ps(1, 9, 17, 25, 33, 41, 49, 57);
    expected[1] = _mm256_setr_ps(2, 10, 18, 26, 34, 42, 50, 58);
    expected[2] = _mm256_setr_ps(3, 11, 19, 27, 35, 43, 51, 59);
    expected[3] = _mm256_setr_ps(4, 12, 20, 28, 36, 44, 52, 60);
    expected[4] = _mm256_setr_ps(5, 13, 21, 29, 37, 45, 53, 61);
    expected[5] = _mm256_setr_ps(6, 14, 22, 30, 38, 46, 54, 62);
    expected[6] = _mm256_setr_ps(7, 15, 23, 31, 39, 47, 55, 63);
    expected[7] = _mm256_setr_ps(8, 16, 24, 32, 40, 48, 56, 64);


    Transpose(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);

    for (U32 i = 0; i < 8; ++i)
        for (U32 j = 0; j < 8; ++j)
            BOOST_CHECK(b[i][j] == expected[i][j]);
}



#endif // __AVX2__
