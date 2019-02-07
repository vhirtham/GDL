#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/utility.h"

#include <array>
#include <iostream>



using namespace GDL;



// __m128 -------------------------------------------------------------------------------------------------------------


//! @brief Tests that the data is (only) written to the register locations specified by the _dst template parameters
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _dst0 = 1, U32 _dst1 = 0, U32 _dst2 = 0, U32 _dst3 = 0>
void TestDotProduct128Destination(__m128 reg0, __m128 reg1, F32 exp0)
{
    __m128 result = sse::DotProduct<_src0, _src1, _src2, _src3, _dst0, _dst1, _dst2, _dst3>(reg0, reg1);

    if (_dst0)
        BOOST_CHECK(sse::GetValue<0>(result) == Approx(exp0));
    else
        BOOST_CHECK(sse::GetValue<0>(result) == ApproxZero<F32>());

    if (_dst1)
        BOOST_CHECK(sse::GetValue<1>(result) == Approx(exp0));
    else
        BOOST_CHECK(sse::GetValue<1>(result) == ApproxZero<F32>());

    if (_dst2)
        BOOST_CHECK(sse::GetValue<2>(result) == Approx(exp0));
    else
        BOOST_CHECK(sse::GetValue<2>(result) == ApproxZero<F32>());

    if (_dst3)
        BOOST_CHECK(sse::GetValue<3>(result) == Approx(exp0));
    else
        BOOST_CHECK(sse::GetValue<3>(result) == ApproxZero<F32>());

    constexpr U32 dst0Next = (_dst0 < 1) ? _dst0 + 1 : 0;
    constexpr U32 dst1Next = (dst0Next == 0) ? (_dst1 < 1) ? _dst1 + 1 : 0 : _dst1;
    constexpr U32 dst2Next = (dst0Next == 0 && dst1Next == 0) ? (_dst2 < 1) ? _dst2 + 1 : 0 : _dst2;
    constexpr U32 dst3Next = (dst0Next == 0 && dst1Next == 0 && dst2Next == 0) ? _dst3 + 1 : _dst3;
    if constexpr (dst3Next < 2)
    {
        TestDotProduct128Destination<_src0, _src1, _src2, _src3, dst0Next, dst1Next, dst2Next, dst3Next>(reg0, reg1,
                                                                                                         exp0);
    }
}



//! @brief Tests the dot product for all possible combinations of sources and destinations
template <U32 _src0 = 1, U32 _src1 = 0, U32 _src2 = 0, U32 _src3 = 0>
void TestDotProduct128()
{

    std::array<F32, 8> testValues0 = {{1, 2, 3, 4}};
    std::array<F32, 8> testValues1 = {{4, 11, 5, 1}};

    __m128 reg0 = _mmx_setzero_p<__m128>();
    __m128 reg1 = _mmx_setzero_p<__m128>();

    for (U32 i = 0; i < sse::numRegisterValues<__m128>; ++i)
    {
        sse::SetValue(reg0, testValues0[i], i);
        sse::SetValue(reg1, testValues1[i], i);
    }

    F32 exp = 0;
    if (_src0)
        exp += testValues0[0] * testValues1[0];
    if (_src1)
        exp += testValues0[1] * testValues1[1];
    if (_src2)
        exp += testValues0[2] * testValues1[2];
    if (_src3)
        exp += testValues0[3] * testValues1[3];

    F32 dotProduct = sse::DotProductF32<_src0, _src1, _src2, _src3>(reg0, reg1);
    BOOST_CHECK(dotProduct == Approx(exp));

    TestDotProduct128Destination<_src0, _src1, _src2, _src3>(reg0, reg1, exp);



    constexpr U32 src0Next = (_src0 < 1) ? _src0 + 1 : 0;
    constexpr U32 src1Next = (src0Next == 0) ? (_src1 < 1) ? _src1 + 1 : 0 : _src1;
    constexpr U32 src2Next = (src0Next == 0 && src1Next == 0) ? (_src2 < 1) ? _src2 + 1 : 0 : _src2;
    constexpr U32 src3Next = (src0Next == 0 && src1Next == 0 && src2Next == 0) ? _src3 + 1 : _src3;
    if constexpr (src3Next < 2)
    {
        TestDotProduct128<src0Next, src1Next, src2Next, src3Next>();
    }
}



BOOST_AUTO_TEST_CASE(Dot_Product_m128)
{
    TestDotProduct128();
}



// __m256 -------------------------------------------------------------------------------------------------------------


//! @brief Tests that the data is (only) written to the register locations specified by the _dst template parameters
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _dst0 = 1, U32 _dst1 = 0, U32 _dst2 = 0, U32 _dst3 = 0>
void TestDotProduct256Destination(__m256 reg0, __m256 reg1, F32 exp0, F32 exp1)
{
    __m256 result = sse::DotProduct<_src0, _src1, _src2, _src3, _dst0, _dst1, _dst2, _dst3>(reg0, reg1);

    if (_dst0)
    {
        BOOST_CHECK(sse::GetValue<0>(result) == Approx(exp0));
        BOOST_CHECK(sse::GetValue<4>(result) == Approx(exp1));
    }
    else
    {
        BOOST_CHECK(sse::GetValue<0>(result) == ApproxZero<F32>());
        BOOST_CHECK(sse::GetValue<4>(result) == ApproxZero<F32>());
    }
    if (_dst1)
    {
        BOOST_CHECK(sse::GetValue<1>(result) == Approx(exp0));
        BOOST_CHECK(sse::GetValue<5>(result) == Approx(exp1));
    }
    else
    {
        BOOST_CHECK(sse::GetValue<1>(result) == ApproxZero<F32>());
        BOOST_CHECK(sse::GetValue<5>(result) == ApproxZero<F32>());
    }
    if (_dst2)
    {
        BOOST_CHECK(sse::GetValue<2>(result) == Approx(exp0));
        BOOST_CHECK(sse::GetValue<6>(result) == Approx(exp1));
    }
    else
    {
        BOOST_CHECK(sse::GetValue<2>(result) == ApproxZero<F32>());
        BOOST_CHECK(sse::GetValue<6>(result) == ApproxZero<F32>());
    }
    if (_dst3)
    {
        BOOST_CHECK(sse::GetValue<3>(result) == Approx(exp0));
        BOOST_CHECK(sse::GetValue<7>(result) == Approx(exp1));
    }
    else
    {
        BOOST_CHECK(sse::GetValue<3>(result) == ApproxZero<F32>());
        BOOST_CHECK(sse::GetValue<7>(result) == ApproxZero<F32>());
    }

    constexpr U32 dst0Next = (_dst0 < 1) ? _dst0 + 1 : 0;
    constexpr U32 dst1Next = (dst0Next == 0) ? (_dst1 < 1) ? _dst1 + 1 : 0 : _dst1;
    constexpr U32 dst2Next = (dst0Next == 0 && dst1Next == 0) ? (_dst2 < 1) ? _dst2 + 1 : 0 : _dst2;
    constexpr U32 dst3Next = (dst0Next == 0 && dst1Next == 0 && dst2Next == 0) ? _dst3 + 1 : _dst3;
    if constexpr (dst3Next < 2)
    {
        TestDotProduct256Destination<_src0, _src1, _src2, _src3, dst0Next, dst1Next, dst2Next, dst3Next>(reg0, reg1,
                                                                                                         exp0, exp1);
    }
}



//! @brief Tests the dot product for all possible combinations of sources and destinations
template <U32 _src0 = 1, U32 _src1 = 0, U32 _src2 = 0, U32 _src3 = 0>
void TestDotProduct256()
{

    std::array<F32, 8> testValues0 = {{1, 2, 3, 4, 5, 6, 7, 8}};
    std::array<F32, 8> testValues1 = {{4, 11, 5, 1, 8, 6, 9, 2}};

    __m256 reg0 = _mmx_setzero_p<__m256>();
    __m256 reg1 = _mmx_setzero_p<__m256>();

    for (U32 i = 0; i < sse::numRegisterValues<__m256>; ++i)
    {
        sse::SetValue(reg0, testValues0[i], i);
        sse::SetValue(reg1, testValues1[i], i);
    }

    F32 exp0 = 0;
    F32 exp1 = 0;
    if (_src0)
    {
        exp0 += testValues0[0] * testValues1[0];
        exp1 += testValues0[4] * testValues1[4];
    }
    if (_src1)
    {
        exp0 += testValues0[1] * testValues1[1];
        exp1 += testValues0[5] * testValues1[5];
    }
    if (_src2)
    {
        exp0 += testValues0[2] * testValues1[2];
        exp1 += testValues0[6] * testValues1[6];
    }
    if (_src3)
    {
        exp0 += testValues0[3] * testValues1[3];
        exp1 += testValues0[7] * testValues1[7];
    }

    TestDotProduct256Destination<_src0, _src1, _src2, _src3>(reg0, reg1, exp0, exp1);



    constexpr U32 src0Next = (_src0 < 1) ? _src0 + 1 : 0;
    constexpr U32 src1Next = (src0Next == 0) ? (_src1 < 1) ? _src1 + 1 : 0 : _src1;
    constexpr U32 src2Next = (src0Next == 0 && src1Next == 0) ? (_src2 < 1) ? _src2 + 1 : 0 : _src2;
    constexpr U32 src3Next = (src0Next == 0 && src1Next == 0 && src2Next == 0) ? _src3 + 1 : _src3;
    if constexpr (src3Next < 2)
    {
        TestDotProduct256<src0Next, src1Next, src2Next, src3Next>();
    }
}



BOOST_AUTO_TEST_CASE(Dot_Product_m256)
{
    TestDotProduct256();
}
