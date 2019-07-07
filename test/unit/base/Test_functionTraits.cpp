#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functionTraits.h"



using namespace GDL;


// --------------------------------------------------------------------------------------------------------------------

U32 TestFunc(const I32& a, F32& b, U32 c)
{
    return static_cast<U32>(a * static_cast<I32>(b)) + c;
}



BOOST_AUTO_TEST_CASE(TestFunctionTraits)
{
    using Func = decltype(&TestFunc);

    constexpr bool checkReturnType = std::is_same_v<typename FunctionTraits<Func>::ReturnType, U32>;
    BOOST_CHECK(checkReturnType);

    constexpr bool checkNumArguments = FunctionTraits<Func>::numArguments == 3;
    BOOST_CHECK(checkNumArguments);

    constexpr bool checkFirstArgumentType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<0>::Type, const I32&>;
    BOOST_CHECK(checkFirstArgumentType);

    constexpr bool checkFirstArgumentValueType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<0>::ValueType, I32>;
    BOOST_CHECK(checkFirstArgumentValueType);

    constexpr bool checkSecondArgumentType = std::is_same_v<typename FunctionTraits<Func>::Argument<1>::Type, F32&>;
    BOOST_CHECK(checkSecondArgumentType);

    constexpr bool checkSecondArgumentValueType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<1>::ValueType, F32>;
    BOOST_CHECK(checkSecondArgumentValueType);

    constexpr bool checkThirdArgumentType = std::is_same_v<typename FunctionTraits<Func>::Argument<2>::Type, U32>;
    BOOST_CHECK(checkThirdArgumentType);

    constexpr bool checkThirdArgumentValueType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<2>::ValueType, U32>;
    BOOST_CHECK(checkThirdArgumentValueType);
}



// --------------------------------------------------------------------------------------------------------------------

struct TestStruct
{
    U32 a = 0;
    F64 b = 1.0;
};



template <typename... _args>
TestStruct TestFuncVariadic(_args...)
{
    return TestStruct();
};



BOOST_AUTO_TEST_CASE(TestFunctionTraits_VariadicFunction)
{
    using Func = decltype(&TestFuncVariadic<const TestStruct&, U32>);

    constexpr bool checkReturnType = std::is_same_v<typename FunctionTraits<Func>::ReturnType, TestStruct>;
    BOOST_CHECK(checkReturnType);

    constexpr bool checkNumArguments = FunctionTraits<Func>::numArguments == 2;
    BOOST_CHECK(checkNumArguments);

    constexpr bool checkFirstArgumentType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<0>::Type, const TestStruct&>;
    BOOST_CHECK(checkFirstArgumentType);

    constexpr bool checkFirstArgumentValueType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<0>::ValueType, TestStruct>;
    BOOST_CHECK(checkFirstArgumentValueType);

    constexpr bool checkSecondArgumentType = std::is_same_v<typename FunctionTraits<Func>::Argument<1>::Type, U32>;
    BOOST_CHECK(checkSecondArgumentType);

    constexpr bool checkSecondArgumentValueType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<1>::ValueType, U32>;
    BOOST_CHECK(checkSecondArgumentValueType);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestFunctionTraits_std_function)
{
    std::function<U32(const U32&, U32&)> function = [](const U32& a, U32& b) { return a * b; };

    using Func = decltype(function);

    constexpr bool checkReturnType = std::is_same_v<typename FunctionTraits<Func>::ReturnType, U32>;
    BOOST_CHECK(checkReturnType);


    constexpr bool checkNumArguments = FunctionTraits<Func>::numArguments == 2;
    BOOST_CHECK(checkNumArguments);

    constexpr bool checkFirstArgumentType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<0>::Type, const U32&>;
    BOOST_CHECK(checkFirstArgumentType);

    constexpr bool checkFirstArgumentValueType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<0>::ValueType, U32>;
    BOOST_CHECK(checkFirstArgumentValueType);

    constexpr bool checkSecondArgumentType = std::is_same_v<typename FunctionTraits<Func>::Argument<1>::Type, U32&>;
    BOOST_CHECK(checkSecondArgumentType);

    constexpr bool checkSecondArgumentValueType =
            std::is_same_v<typename FunctionTraits<Func>::Argument<1>::ValueType, U32>;
    BOOST_CHECK(checkSecondArgumentValueType);
}
