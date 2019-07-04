#pragma once

#include "gdl/math/solver/pivot.h"

#include "gdl/math/serial/matSerial.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/base/simd/swizzle.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/simd/vecSIMD.h"

#include <cmath>

namespace GDL::Solver
{




// --------------------------------------------------------------------------------------------------------------------

template <typename _type, I32 _size>
inline U32 PivotDenseSerial<_type, _size>::FindPivot(U32 iteration, const MatrixDataArray& matData)
{
    const U32 colStartIdx = iteration * _size;
    F32 maxAbs = std::abs(matData[colStartIdx + iteration]);
    U32 maxValIdx = iteration;
    for (U32 i = iteration + 1; i < _size; ++i)
    {
        F32 cmpAbs = std::abs(matData[i + colStartIdx]);
        if (maxAbs < cmpAbs)
        {
            maxAbs = cmpAbs;
            maxValIdx = i;
        }
    }

    return maxValIdx;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, I32 _size>
inline void PivotDenseSerial<_type, _size>::PivotingStep(U32 iteration, MatrixDataArray& matData,
                                                         VectorDataArray& vecData)
{
    U32 pivotRowIdx = FindPivot(iteration, matData);

    if (pivotRowIdx != iteration)
        SwapPivot(pivotRowIdx, iteration, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, I32 _size>
inline void PivotDenseSerial<_type, _size>::SwapPivot(U32 pivotRowIdx, U32 iteration, MatrixDataArray& matData,
                                                      VectorDataArray& vecData)
{
    DEV_EXCEPTION(pivotRowIdx < iteration,
                  "Internal error. Row of the pivot element must be higher or equal to the current iteration number");

    const U32 rowDiff = pivotRowIdx - iteration;
    for (U32 i = iteration * _size + iteration; i < matData.size(); i += _size)
        std::swap(matData[i], matData[i + rowDiff]);

    std::swap(vecData[iteration], vecData[pivotRowIdx]);
}







// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline U32 PivotDenseSSE<_registerType, _size>::FindPivot(U32 iteration, U32 regRowIdx, const MatrixDataArray& matData)
{
    using namespace GDL::simd;

    const U32 colStartIdx = iteration * numColRegisters;


    // Vectorized comparisons
    _registerType zero = _mm_setzero<_registerType>();
    _registerType cmpAbs = Abs(BlendAboveIndex<_regValueIdx>(matData[colStartIdx + regRowIdx], zero));
    _registerType cmpIdx = _mm_set1<_registerType>(regRowIdx);
    for (U32 i = regRowIdx + 1; i < numColRegisters; ++i)
    {
        _registerType cmpAbs2 = Abs(matData[colStartIdx + i]);
        _registerType cmpRes = _mm_cmplt(cmpAbs, cmpAbs2);
        cmpAbs = _mm_blendv(cmpAbs, cmpAbs2, cmpRes);
        cmpIdx = _mm_blendv(cmpIdx, _mm_set1<_registerType>(i), cmpRes);
    }

    // Find pivot in result register
    alignas(alignment) std::array<ValueType, numRegisterValues> values2;
    _mm_store(values2.data(), cmpAbs);
    ValueType maxVal = values2[0];
    U32 maxValIdx = 0;
    for (U32 i = 1; i < numRegisterValues; ++i)
        if (maxVal < values2[i])
        {
            maxVal = values2[i];
            maxValIdx = i;
        }


    return GetValue(cmpIdx, maxValIdx) * numRegisterValues + maxValIdx;
}






// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void PivotDenseSSE<_registerType, _size>::PivotingStepRegister(U32 iteration, U32 regRowIdx,
                                                                      MatrixDataArray& matData,
                                                                      VectorDataArray& vecData)
{
    U32 pivotRowIdx = FindPivot<_regValueIdx>(iteration, regRowIdx, matData);

    DEV_EXCEPTION(pivotRowIdx >= _size, "Internal error. Pivot index bigger than matrix size.");

    if (pivotRowIdx != iteration)
        SwapPivot<_regValueIdx>(pivotRowIdx, iteration, regRowIdx, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void PivotDenseSSE<_registerType, _size>::SwapPivot(U32 pivotRowIdx, U32 iteration, U32 regRowIdx,
                                                           MatrixDataArray& matData, VectorDataArray& vecData)
{
    const U32 pivotRegRowIdx = pivotRowIdx / numRegisterValues;
    if (regRowIdx == pivotRegRowIdx)
        SwapPivotSwitch<_regValueIdx, true>(pivotRowIdx, iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
    else
        SwapPivotSwitch<_regValueIdx, false>(pivotRowIdx, iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regIdxDst, U32 _regIdxPiv, bool _sameReg>
inline void PivotDenseSSE<_registerType, _size>::SwapPivotAllColumns(U32 iteration, U32 pivotRegRowIdx, U32 regRowIdx,
                                                                     MatrixDataArray& matData, VectorDataArray& vecData)
{
    using namespace GDL::simd;

    if constexpr (_sameReg)
    {
        if constexpr (_regIdxDst < _regIdxPiv)
        {
            for (U32 i = iteration * numColRegisters + regRowIdx; i < matData.size(); i += numColRegisters)
                matData[i] = Swap<_regIdxDst, _regIdxPiv>(matData[i]);
            vecData[regRowIdx] = Swap<_regIdxDst, _regIdxPiv>(vecData[regRowIdx]);
        }
        else
            THROW("Internal index error - Probably singular matrix."); // LCOV_EXCL_LINE
    }
    else
    {
        DEV_EXCEPTION(pivotRegRowIdx <= regRowIdx,
                      "Internal error. Pivot register has equal or lower index than current register");
        DEV_EXCEPTION(pivotRegRowIdx / numColRegisters != regRowIdx / numColRegisters,
                      "Internal error. Registers are not in the same column.");

        U32 regDiff = pivotRegRowIdx - regRowIdx;

        for (U32 i = iteration * numColRegisters + regRowIdx; i < matData.size(); i += numColRegisters)
            Exchange<_regIdxDst, _regIdxPiv>(matData[i], matData[i + regDiff]);
        Exchange<_regIdxDst, _regIdxPiv>(vecData[regRowIdx], vecData[regRowIdx + regDiff]);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx, bool _sameReg>
inline void PivotDenseSSE<_registerType, _size>::SwapPivotSwitch(U32 pivotRowIdx, U32 iteration, U32 pivotRegRowIdx,
                                                                 U32 regRowIdx, MatrixDataArray& matData,
                                                                 VectorDataArray& vecData)
{
    static_assert(numRegisterValues == 2 || numRegisterValues == 4 || numRegisterValues == 8,
                  "Only registers with 2,4 or 8 values are supported.");

    if constexpr (numRegisterValues == 2)
        switch (pivotRowIdx % numRegisterValues)
        {
        case 0:
            SwapPivotAllColumns<_regValueIdx, 0, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 1:
            SwapPivotAllColumns<_regValueIdx, 1, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        }

    if constexpr (numRegisterValues == 4)
        switch (pivotRowIdx % numRegisterValues)
        {
        case 0:
            SwapPivotAllColumns<_regValueIdx, 0, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 1:
            SwapPivotAllColumns<_regValueIdx, 1, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 2:
            SwapPivotAllColumns<_regValueIdx, 2, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 3:
            SwapPivotAllColumns<_regValueIdx, 3, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        }

    if constexpr (numRegisterValues == 8)
        switch (pivotRowIdx % numRegisterValues)
        {
        case 0:
            SwapPivotAllColumns<_regValueIdx, 0, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 1:
            SwapPivotAllColumns<_regValueIdx, 1, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 2:
            SwapPivotAllColumns<_regValueIdx, 2, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 3:
            SwapPivotAllColumns<_regValueIdx, 3, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 4:
            SwapPivotAllColumns<_regValueIdx, 4, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 5:
            SwapPivotAllColumns<_regValueIdx, 5, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 6:
            SwapPivotAllColumns<_regValueIdx, 6, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 7:
            SwapPivotAllColumns<_regValueIdx, 7, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        }
}


} // namespace GDL::Solver
