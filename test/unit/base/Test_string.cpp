#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/utility/heapAllocationCounter.h"
#include "gdl/base/string.h"

#include <iostream>
#include <cstdio>

using namespace GDL;

template <typename _stringType>
void TestAppendToString()
{
    _stringType s1{"This"};
    _stringType s2{"whole"};

    AppendToString(s1, " is the ", s2, " text.");
    BOOST_CHECK(s1.compare("This is the whole text.") == 0);
}


BOOST_AUTO_TEST_CASE(Append_to_string)
{
#ifndef USE_STD_ALLOCATOR
    MemoryManager& mm = MemoryManager::Instance();
    mm.CreateGeneralPurposeMemory(1_MB);
    mm.CreateMemoryStack(1_MB);
    mm.EnableThreadPrivateMemory();
    mm.Initialize();
    mm.CreatePrivateMemoryStackForThisThread(1_MB);
#endif

    TestAppendToString<std::string>();
    TestAppendToString<String>();
    TestAppendToString<StringS>();
    TestAppendToString<StringTPS>();


#ifndef USE_STD_ALLOCATOR
    mm.DeletePrivateMemoryStackForThisThread();
    mm.Deinitialize();
#endif
}



template <typename _stringType>
void TestMakeString()
{
    _stringType s2{"whole"};

    _stringType s1 = MakeString<_stringType>("This is the ", s2, " text.");
    BOOST_CHECK(s1.compare("This is the whole text.") == 0);
}



BOOST_AUTO_TEST_CASE(Make_string)
{
#ifndef USE_STD_ALLOCATOR
    MemoryManager& mm = MemoryManager::Instance();
    mm.Initialize();
    mm.CreatePrivateMemoryStackForThisThread(1_MB);
#endif

    TestMakeString<std::string>();
    TestMakeString<String>();
    TestMakeString<StringS>();
    TestMakeString<StringTPS>();


#ifndef USE_STD_ALLOCATOR
    mm.DeletePrivateMemoryStackForThisThread();
    mm.Deinitialize();
#endif
}



BOOST_AUTO_TEST_CASE(To_String)
{
#ifndef USE_STD_ALLOCATOR
    MemoryManager& mm = MemoryManager::Instance();
    mm.Initialize();
#endif
    HeapAllocationCounter hac;
    {
        // floating point types
        F32 f32 = -135.2398f;
        BOOST_CHECK(ToString(std::abs(f32)).compare("135.2398") == 0);
        BOOST_CHECK(ToString(f32).compare("-135.2398") == 0);

        F64 f64 = -951463784994.1234;
        BOOST_CHECK(ToString(std::abs(f64)).compare("951463784994.1234") == 0);
        BOOST_CHECK(ToString(f64).compare("-951463784994.1234") == 0);


        // integers
        I8 i8 = static_cast<I8>(-121);
        BOOST_CHECK(ToString(static_cast<I8>(std::abs(i8))).compare("121") == 0);
        BOOST_CHECK(ToString(i8).compare("-121") == 0);

        I16 i16 = static_cast<I16>(-25398);
        BOOST_CHECK(ToString(static_cast<I16>(std::abs(i16))).compare("25398") == 0);
        BOOST_CHECK(ToString(i16).compare("-25398") == 0);

        I32 i32 = -1958501683;
        BOOST_CHECK(ToString(std::abs(i32)).compare("1958501683") == 0);
        BOOST_CHECK(ToString(i32).compare("-1958501683") == 0);

        I64 i64 = -1207927512116994732;
        BOOST_CHECK(ToString(std::abs(i64)).compare("1207927512116994732") == 0);
        BOOST_CHECK(ToString(i64).compare("-1207927512116994732") == 0);



        // unsigned integers
        U8 u8 = static_cast<U8>(215);
        BOOST_CHECK(ToString(u8).compare("215") == 0);

        U16 u16 = static_cast<U16>(55398);
        BOOST_CHECK(ToString(u16).compare("55398") == 0);

        U32 u32 = 3958501683;
        BOOST_CHECK(ToString(u32).compare("3958501683") == 0);

        U64 u64 = 3207927512116994732;
        BOOST_CHECK(ToString(u64).compare("3207927512116994732") == 0);

        std::to_string(2);
    }
    BOOST_CHECK(hac.CheckNumCallsExpectedCustomAllocation(0, 0));

#ifndef USE_STD_ALLOCATOR
    mm.Deinitialize();
#endif
}
