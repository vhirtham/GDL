#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/sum.h"
#include "gdl/base/simd/utility.h"

using namespace GDL;
using namespace GDL::simd;



template <typename _registerType>
void TestRegisterSum()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    _registerType a;
    Type sum = 0;

    for (U32 i = 0; i < numRegVals; ++i)
    {
        sum += (i + 1);
        SetValue(a, i, static_cast<Type>(i + 1));
    }

    _registerType b = Sum(a);

    for (U32 i = 0; i < numRegVals; ++i)
        BOOST_CHECK(GetValue(b, i) == Approx(sum));
}



BOOST_AUTO_TEST_CASE(Register_sum_m128)
{
    TestRegisterSum<__m128>();
}



BOOST_AUTO_TEST_CASE(Register_sum_m128d)
{
    TestRegisterSum<__m128d>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(Register_sum_m256)
{
    TestRegisterSum<__m256>();
}



BOOST_AUTO_TEST_CASE(Register_sum_m256d)
{
    TestRegisterSum<__m256d>();
}

#endif // __AVX2__
