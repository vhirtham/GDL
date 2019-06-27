#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/simd/abs.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/utility.h"

using namespace GDL;

//! @brief Tests the SSEAbs function
//! @remark The direct comparison of floating point values is intended only the sign bit should be modified
template <typename _registerType, U32 _count = 1>
void TestAbs()
{
    using DataType = decltype(simd::GetDataType<_registerType>());
    constexpr U32 numRegisterEntries = simd::numRegisterValues<_registerType>;

    auto cmpAllElementsEqual = [](_registerType lhs, _registerType rhs) {
        for (U32 i = 0; i < numRegisterEntries; ++i)
            if (simd::GetValue(lhs, i) != Approx(simd::GetValue(rhs, i)))
                return false;
        return true;
    };

    alignas(simd::alignmentBytes<_registerType>) _registerType ref = _mm_setzero<_registerType>();
    alignas(simd::alignmentBytes<_registerType>) _registerType cmp = _mm_setzero<_registerType>();
    alignas(simd::alignmentBytes<_registerType>) _registerType cmp2 = _mm_setzero<_registerType>();
    alignas(simd::alignmentBytes<_registerType>) _registerType cmp3 = _mm_setzero<_registerType>();

    for (I32 i = 0; i < static_cast<I32>(numRegisterEntries); ++i)
    {

        simd::SetValue(ref, static_cast<U32>(i), static_cast<DataType>(i));
        simd::SetValue(cmp, static_cast<U32>(i), static_cast<DataType>(-i));
        simd::SetValue(cmp2, static_cast<U32>(i), static_cast<DataType>(i * ((i % 2 == 0) ? 1 : -1)));
        simd::SetValue(cmp3, static_cast<U32>(i), static_cast<DataType>(i * ((i % 3 == 0) ? 1 : -1)));
    }

    BOOST_CHECK(cmpAllElementsEqual(ref, simd::Abs(ref)));
    BOOST_CHECK(!cmpAllElementsEqual(ref, cmp));
    BOOST_CHECK(cmpAllElementsEqual(ref, simd::Abs(cmp)));
    BOOST_CHECK(!cmpAllElementsEqual(ref, cmp2));
    BOOST_CHECK(cmpAllElementsEqual(ref, simd::Abs(cmp2)));
    BOOST_CHECK(!cmpAllElementsEqual(ref, cmp3));
    BOOST_CHECK(cmpAllElementsEqual(ref, simd::Abs(cmp3)));
}



BOOST_AUTO_TEST_CASE(Abs)
{
    TestAbs<__m128>();
    TestAbs<__m128d>();
#ifdef __AVX2__
    TestAbs<__m256>();
    TestAbs<__m256d>();
#endif // __AVX2__
}
