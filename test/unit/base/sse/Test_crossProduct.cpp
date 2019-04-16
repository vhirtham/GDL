#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/directAccess.h"

using namespace GDL;
using namespace GDL::sse;

F32 DotProduct(std::array<F32, 3> a, std::array<F32, 3> b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


void Test_CrossProductResult(std::array<F32, 3> aData, std::array<F32, 3> bData, std::array<F32, 3> expData)
{
    __m128 a = _mm_setr_ps(aData[0], aData[1], aData[2], 0);
    __m128 b = _mm_setr_ps(bData[0], bData[1], bData[2], 0);

    __m128 c = CrossProduct(a, b);
    __m128 d = CrossProduct(b, a);
    std::array<F32, 3> cData = {{GetValue<0>(c), GetValue<1>(c), GetValue<2>(c)}};
    std::array<F32, 3> dData = {{GetValue<0>(d), GetValue<1>(d), GetValue<2>(d)}};

    BOOST_CHECK(DotProduct(aData, cData) == ApproxZero<F32>());
    BOOST_CHECK(DotProduct(bData, cData) == ApproxZero<F32>());
    BOOST_CHECK(DotProduct(aData, dData) == ApproxZero<F32>());
    BOOST_CHECK(DotProduct(bData, dData) == ApproxZero<F32>());

    BOOST_CHECK(GetValue<0>(c) == Approx(expData[0]));
    BOOST_CHECK(GetValue<1>(c) == Approx(expData[1]));
    BOOST_CHECK(GetValue<2>(c) == Approx(expData[2]));
    BOOST_CHECK(GetValue<3>(c) == ApproxZero<F32>());
    BOOST_CHECK(GetValue<0>(d) == Approx(-expData[0]));
    BOOST_CHECK(GetValue<1>(d) == Approx(-expData[1]));
    BOOST_CHECK(GetValue<2>(d) == Approx(-expData[2]));
    BOOST_CHECK(GetValue<3>(d) == ApproxZero<F32>());
}



BOOST_AUTO_TEST_CASE(Test_Cross_Product_Result)
{
    Test_CrossProductResult({{1, 0, 0}}, {{0, 1, 0}}, {{0, 0, 1}});
    Test_CrossProductResult({{0, 0, 1}}, {{1, 0, 0}}, {{0, 1, 0}});
    Test_CrossProductResult({{0, 1, 0}}, {{0, 0, 1}}, {{1, 0, 0}});
    Test_CrossProductResult({{3, 1, 4}}, {{2, 6, 1}}, {{-23, 5, 16}});
    Test_CrossProductResult({{1, 3, 1}}, {{5, 2, 8}}, {{22, -3, -13}});
    Test_CrossProductResult({{4, 4, 4}}, {{3, 1, 2}}, {{4, 4, -8}});
    Test_CrossProductResult({{8, 1, 6}}, {{1, 2, 3}}, {{-9, -18, 15}});
}
