#include "gdl/GDLTypedefs.h"
#include "gdl/resources/memory/internalStackAllocator.h"

#include <benchmark/benchmark.h>

#include <array>
#include <map>
#include <vector>

using namespace GDL;

constexpr int numEmplaces = 100;
constexpr int arraySize = 1000;

// Fixture declaration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class STDAlloc : public benchmark::Fixture
{
public:
    void pushBack()
    {
        std::vector<std::array<F32, arraySize>> v;
        for (I32 i = 0; i < numEmplaces; ++i)
            v.push_back(std::array<F32, arraySize>());
    }

    void emplaceSmall()
    {
        std::map<I32, F32> m;
        for (I32 i = 0; i < numEmplaces; ++i)
            m.emplace(i, static_cast<F32>(i));
    }

    void emplaceBig()
    {
        std::map<I32, std::array<F32, arraySize>> m;
        for (I32 i = 0; i < numEmplaces; ++i)
            m.emplace(i, std::array<F32, arraySize>());
    }

    STDAlloc()
    {
    }
};


class ISA : public benchmark::Fixture
{
public:
    void pushBack()
    {
        std::vector<std::array<F32, arraySize>, InternalStackAllocator<std::array<F32, arraySize>>> v;
        for (I32 i = 0; i < numEmplaces; ++i)
            v.push_back(std::array<F32, arraySize>());
    }

    void emplaceSmall()
    {
        std::map<I32, F32, std::less<I32>, InternalStackAllocator<std::pair<I32, F32>>> m;
        for (I32 i = 0; i < numEmplaces; ++i)
            m.emplace(i, static_cast<F32>(i));
    }

    void emplaceBig()
    {
        std::map<I32, std::array<F32, arraySize>, std::less<I32>,
                 InternalStackAllocator<std::pair<I32, std::array<F32, arraySize>>>>
                m;
        for (I32 i = 0; i < numEmplaces; ++i)
            m.emplace(i, std::array<F32, arraySize>());
    }

    ISA()
    {
        std::vector<F32, InternalStackAllocator<F32>> vInit;
        vInit.push_back(1.0); // Force Memory stack allocation - implement better way
    }
};


// Map emplace small %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


BENCHMARK_F(STDAlloc, STDAlloc_Emplace_Small)(benchmark::State& state)
{
    for (auto _ : state)
        emplaceSmall();
}


BENCHMARK_F(ISA, ISA_Emplace_Small)(benchmark::State& state)
{
    for (auto _ : state)
        emplaceSmall();
}


// Map emplace big %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(STDAlloc, STDAlloc_Emplace_Big)(benchmark::State& state)
{
    for (auto _ : state)
        emplaceBig();
}


BENCHMARK_F(ISA, ISA_Emplace_Big)(benchmark::State& state)
{
    for (auto _ : state)
        emplaceBig();
}

// Vector push back %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


BENCHMARK_F(STDAlloc, STDAlloc_Vector_Push_Back_Big)(benchmark::State& state)
{
    for (auto _ : state)
        pushBack();
}


BENCHMARK_F(ISA, ISA_Vector_Vector_Push_Back_Big)(benchmark::State& state)
{
    for (auto _ : state)
        pushBack();
}



// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_MAIN();
