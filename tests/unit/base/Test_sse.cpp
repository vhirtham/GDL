#include <boost/test/unit_test.hpp>


#include "gdl/base/functions/sse.h"


using namespace GDL;


BOOST_AUTO_TEST_CASE(Calc_Min_Num_Array_Registers)
{
    BOOST_CHECK(CalcMinNumArrayRegisters<__m128>(13) == 4);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m128>(4) == 1);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m128d>(13) == 7);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m128d>(4) == 2);
#ifdef __AVX2__
    BOOST_CHECK(CalcMinNumArrayRegisters<__m256>(13) == 2);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m256>(4) == 1);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m256d>(13) == 4);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m256d>(4) == 1);
#ifdef __AVX512F__
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512>(21) == 2);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512>(13) == 1);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512>(4) == 1);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512d>(21) == 3);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512d>(13) == 2);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512d>(4) == 1);
#endif // __AVX512F__
#endif // __AVX2__
}
