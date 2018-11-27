#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/utility.h"

using namespace GDL;

//! @brief Tests the SSEAbs function
//! @remark The direct comparison of floating point values is intended only the sign bit should be modified
template <typename _registerType, U32 _count = 1>
void TestAbs()
{
    using DataType = decltype(sse::GetDataType<_registerType>());
    constexpr U32 numRegisterEntries = sse::numRegisterValues<_registerType>;

    auto cmpAllElementsEqual = [](_registerType lhs, _registerType rhs) {
        for (U32 i = 0; i < numRegisterEntries; ++i)
            if (sse::GetValue(lhs, i) != Approx(sse::GetValue(rhs, i)))
                return false;
        return true;
    };

    alignas(sse::alignmentBytes<_registerType>) _registerType ref = _mmx_setzero_p<_registerType>();
    alignas(sse::alignmentBytes<_registerType>) _registerType cmp = _mmx_setzero_p<_registerType>();
    alignas(sse::alignmentBytes<_registerType>) _registerType cmp2 = _mmx_setzero_p<_registerType>();
    alignas(sse::alignmentBytes<_registerType>) _registerType cmp3 = _mmx_setzero_p<_registerType>();

    for (I32 i = 0; i < static_cast<I32>(numRegisterEntries); ++i)
    {

        sse::SetValue(ref, static_cast<DataType>(i), static_cast<U32>(i));
        sse::SetValue(cmp, static_cast<DataType>(-i), static_cast<U32>(i));
        sse::SetValue(cmp2, static_cast<DataType>(i * ((i % 2 == 0) ? 1 : -1)), static_cast<U32>(i));
        sse::SetValue(cmp3, static_cast<DataType>(i * ((i % 3 == 0) ? 1 : -1)), static_cast<U32>(i));
    }

    BOOST_CHECK(cmpAllElementsEqual(ref, sse::Abs(ref)));
    BOOST_CHECK(!cmpAllElementsEqual(ref, cmp));
    BOOST_CHECK(cmpAllElementsEqual(ref, sse::Abs(cmp)));
    BOOST_CHECK(!cmpAllElementsEqual(ref, cmp2));
    BOOST_CHECK(cmpAllElementsEqual(ref, sse::Abs(cmp2)));
    BOOST_CHECK(!cmpAllElementsEqual(ref, cmp3));
    BOOST_CHECK(cmpAllElementsEqual(ref, sse::Abs(cmp3)));
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
