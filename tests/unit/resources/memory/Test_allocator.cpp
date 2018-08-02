#include <boost/test/unit_test.hpp>

#include "gdl/GDLTypedefs.h"
#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/memoryManager.h"


#include <iostream>
#include <vector>

using namespace GDL;


// template <template <typename> class Allocator = std::allocator>
// struct test2
//{
//    std::vector<int, Allocator<int>> mVec;
//};

// template <typename TAllocator>
// void someFunc(const std::vector<GDL::F32, TAllocator>& A)
//{
//    std::cout << "can do it" << std::endl;
//}


BOOST_AUTO_TEST_CASE(Vector)
{
    auto& mM = MemoryManager::Instance();
    mM.CreateGeneralPurposeMemory(4096);
    mM.Initialize();

    std::vector<F32, GeneralPurposeAllocator<F32>> vec;
    vec.resize(7);
    for (U32 i = 0; i < vec.size(); ++i)
        vec[i] = i * 2;
    for (U32 i = 0; i < vec.size(); ++i)
        std::cout << vec[i] << std::endl;
    vec.clear();
    vec.shrink_to_fit();

    mM.Deinitialize();
}


BOOST_AUTO_TEST_CASE(Map)
{
    auto& mM = MemoryManager::Instance();
    mM.Initialize();

    std::cout << sizeof(std::pair<const I32, F32>) << std::endl;

    std::map<I32, F32, std::less<I32>, GeneralPurposeAllocator<std::pair<const I32, F32>>> m;

    for (I32 i = 0; i < 20; ++i)
        m.emplace(i, static_cast<F32>(i));

    for (I32 i = 0; i < 20; ++i)
        BOOST_CHECK(m.find(i) != m.end());

    m.clear();

    mM.Deinitialize();
}
