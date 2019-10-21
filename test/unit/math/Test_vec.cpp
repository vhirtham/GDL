#include <boost/test/unit_test.hpp>

#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/serial/vecSerial.h"

#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;


// Fixture definition -------------------------------------------------------------------------------------------------

template <template <typename, U32, bool> class _vector, typename _type, bool _isCol>
struct Fixture
{
    _vector<_type, 16, _isCol> a =
            _vector<_type, 16, _isCol>(0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.);
};



// Construction and Data function -------------------------------------------------------------------------------------

template <template <typename, U32, bool> class _vector, typename _type, bool _isClol>
void CtorDataTest()
{
    _vector<_type, 16, _isClol> a;
    BOOST_CHECK(CheckArrayZero(a.Data()));

    _vector<_type, 5, _isClol> b(0., 1., 2., 3., 4.);
    std::array<_type, 5> expB{{0., 1., 2., 3., 4.}};
    BOOST_CHECK(CheckCloseArray(b.Data(), expB));

    _vector<_type, 5, _isClol> b1(expB);
    BOOST_CHECK(CheckCloseArray(b1.Data(), expB));
}


template <typename _type, bool _isCol>
void SSECtorTest()
{
    constexpr U32 numVectorVals = 9;
    using RegisterType = typename VecSIMD<_type, numVectorVals>::RegisterType;


    constexpr U32 numArrayReg = simd::CalcMinNumArrayRegisters<RegisterType>(numVectorVals);
    constexpr U32 numRegVals = simd::numRegisterValues<RegisterType>;

    std::array<RegisterType, numArrayReg> arr{{_mm_setzero<RegisterType>()}};
    for (U32 i = 0; i < numArrayReg; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            simd::SetValue(arr[i], j, static_cast<_type>(i * numRegVals + j));

    VecSIMD<_type, numVectorVals, _isCol> a(arr);

    std::array<_type, numVectorVals> expA;
    for (U32 i = 0; i < numVectorVals; ++i)
        expA[i] = static_cast<_type>(i);

    BOOST_CHECK(CheckCloseArray(a.Data(), expA));
}



BOOST_AUTO_TEST_CASE(Construction_Serial)
{
    CtorDataTest<VecSerial, F32, true>();
    CtorDataTest<VecSerial, F32, false>();
    CtorDataTest<VecSerial, F64, true>();
    CtorDataTest<VecSerial, F64, false>();
}



BOOST_AUTO_TEST_CASE(Construction_SSE)
{
    CtorDataTest<VecSIMD, F32, true>();
    CtorDataTest<VecSIMD, F32, false>();
    CtorDataTest<VecSIMD, F64, true>();
    CtorDataTest<VecSIMD, F64, false>();
    SSECtorTest<F32, true>();
    SSECtorTest<F32, false>();
    SSECtorTest<F64, true>();
    SSECtorTest<F64, false>();
}



// Operator[] test ----------------------------------------------------------------------------------------------------

template <template <typename, U32, bool> class _vector, typename _type, U32 _size, bool _isCol>
void DirectAccessOperatorVectorTest(const _vector<_type, _size, _isCol>& vector)
{
    std::array<_type, _size> data = vector.Data();

    for (U32 i = 0; i < _size; ++i)
        BOOST_CHECK(vector[i] == Approx(data[i]));

    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] _type test = vector[_size + 1], Exception);
}



template <template <typename, U32, bool> class _vector, typename _type>
void DirectAccessOperatorTest()
{
    Fixture<_vector, _type, true> fixtureC;
    DirectAccessOperatorVectorTest<_vector>(fixtureC.a);

    Fixture<_vector, _type, true> fixtureR;
    DirectAccessOperatorVectorTest<_vector>(fixtureR.a);
}



BOOST_AUTO_TEST_CASE(Direct_Access_Serial)
{
    DirectAccessOperatorTest<VecSerial, F32>();
    DirectAccessOperatorTest<VecSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Direct_Access_SSE)
{
    DirectAccessOperatorTest<VecSIMD, F32>();
    DirectAccessOperatorTest<VecSIMD, F64>();
}
