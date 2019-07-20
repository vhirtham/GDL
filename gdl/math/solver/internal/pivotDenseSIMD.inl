#pragma once

#include "gdl/math/solver/internal/pivotDenseSIMD.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/swizzle.h"


namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
inline PivotDenseSSE<_registerType, _size>::VectorPermutationData::VectorPermutationData(U32 idx0, U32 idx1,
                                                                                         PermutationFuncPtr funcPtr)
    : mRegIdx0{idx0}
    , mRegIdx1{idx1}
    , mFuncPtr{funcPtr}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
inline void PivotDenseSSE<_registerType, _size>::VectorPermutationDataArray::AddPermutation(U32 idx0, U32 idx1,
                                                                                            PermutationFuncPtr funcPtr)
{
    mPermutations[mNumPermutations] = VectorPermutationData(idx0, idx1, funcPtr);
    ++mNumPermutations;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
template <U32 _regElmIdxPiv>
inline U32 PivotDenseSSE<_registerType, _size>::FindMaxAbsValueCol(U32 iteration, U32 regRowIdxPiv,
                                                                   const MatrixDataArray& matData)
{
    using namespace GDL::simd;

    const U32 colStartIdx = iteration * numColRegisters;


    // Vectorized comparisons
    _registerType zero = _mm_setzero<_registerType>();
    _registerType cmpAbs = Abs(BlendAboveIndex<_regElmIdxPiv>(matData[colStartIdx + regRowIdxPiv], zero));
    _registerType cmpIdx = _mm_set1<_registerType>(regRowIdxPiv);

    for (U32 i = regRowIdxPiv + 1; i < numColRegisters; ++i)
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

template <typename _registerType, U32 _size>
template <U32 _regElmIdxPiv, typename _typeVecPerm>
inline void PivotDenseSSE<_registerType, _size>::PartialPivotingStepRegister(U32 iteration, U32 regRowIdxPiv,
                                                                             MatrixDataArray& matData,
                                                                             _typeVecPerm& vecPermData)
{
    U32 rowIdxSwp = FindMaxAbsValueCol<_regElmIdxPiv>(iteration, regRowIdxPiv, matData);

    DEV_EXCEPTION(rowIdxSwp >= _size, "Internal error. Pivot index bigger than matrix size.");

    if (rowIdxSwp != iteration)
        SwapRowPivot<_regElmIdxPiv>(rowIdxSwp, iteration, regRowIdxPiv, matData, vecPermData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
template <U32 _regElmIdx0, U32 _regElmIdx1, bool _sameReg>
inline void PivotDenseSSE<_registerType, _size>::PermutationFunction(_registerType& reg0,
                                                                     [[maybe_unused]] _registerType& reg1)
{
    using namespace GDL::simd;

    if constexpr (_sameReg)
        reg0 = Swap<_regElmIdx0, _regElmIdx1>(reg0);
    else
        Exchange<_regElmIdx0, _regElmIdx1>(reg0, reg1);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
inline void PivotDenseSSE<_registerType, _size>::PermuteVector(VectorDataArray& vectorData,
                                                               const VectorPermutationDataArray& permutationData)
{
    for (U32 i = 0; i < permutationData.mNumPermutations; ++i)
    {
        U32 idx0 = permutationData.mPermutations[i].mRegIdx0;
        U32 idx1 = permutationData.mPermutations[i].mRegIdx1;
        permutationData.mPermutations[i].mFuncPtr(vectorData[idx0], vectorData[idx1]);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
template <U32 _regElmIdxPiv, Pivot _pivot, typename _typeVecPerm>
inline void PivotDenseSSE<_registerType, _size>::PivotingStepRegister(U32 iteration, U32 regRowIdxPiv,
                                                                      MatrixDataArray& matData,
                                                                      _typeVecPerm& vecPermData)
{
    static_assert(_pivot != Pivot::NONE, "Unneccessary function call");

    static_assert(_pivot == Pivot::PARTIAL, "Unsupported pivoting strategy");

    if constexpr (_pivot == Pivot::PARTIAL)
        PartialPivotingStepRegister<_regElmIdxPiv>(iteration, regRowIdxPiv, matData, vecPermData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
template <U32 _regElmIdxPiv, typename _typeVecPerm>
inline void PivotDenseSSE<_registerType, _size>::SwapRowPivot(U32 rowIdxSwp, U32 iteration, U32 regRowIdxPiv,
                                                              MatrixDataArray& matData, _typeVecPerm& vecPermData)
{
    const U32 regRowIdxSwp = rowIdxSwp / numRegisterValues;

    if (regRowIdxPiv == regRowIdxSwp)
        SwapRowPivotSwitch<_regElmIdxPiv, true>(rowIdxSwp, iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
    else
        SwapRowPivotSwitch<_regElmIdxPiv, false>(rowIdxSwp, iteration, regRowIdxPiv, regRowIdxSwp, matData,
                                                 vecPermData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
template <U32 _regElmIdxPiv, U32 _regElmIdxSwp, bool _sameReg, typename _typeVecPerm>
inline void PivotDenseSSE<_registerType, _size>::SwapRowPivotLoop(U32 iteration, U32 regRowIdxPiv, U32 regRowIdxSwp,
                                                                  MatrixDataArray& matData, _typeVecPerm& vecPermData)
{
    using namespace GDL::simd;

    static_assert(std::is_same<_typeVecPerm, VectorDataArray>::value ||
                          std::is_same<_typeVecPerm, VectorPermutationDataArray>::value,
                  "Invalid parameter type");

    if constexpr (_sameReg)
        SwapRowPivotLoopSameReg<_regElmIdxPiv, _regElmIdxSwp>(iteration, regRowIdxPiv, matData, vecPermData);
    else
        SwapRowPivotLoopDiffReg<_regElmIdxPiv, _regElmIdxSwp>(iteration, regRowIdxPiv, regRowIdxSwp, matData,
                                                              vecPermData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
template <U32 _regElmIdxPiv, U32 _regElmIdxSwp, typename _typeVecPerm>
inline void PivotDenseSSE<_registerType, _size>::SwapRowPivotLoopDiffReg(U32 iteration, U32 regRowIdxPiv,
                                                                         U32 regRowIdxSwp, MatrixDataArray& matData,
                                                                         _typeVecPerm& vecPermData)
{
    using namespace GDL::simd;

    DEV_EXCEPTION(regRowIdxSwp <= regRowIdxPiv,
                  "Internal error. Pivot register has equal or lower index than current register");
    DEV_EXCEPTION(regRowIdxSwp / numColRegisters != regRowIdxPiv / numColRegisters,
                  "Internal error. Registers are not in the same column.");


    constexpr bool dataIsVectorData = std::is_same<_typeVecPerm, VectorDataArray>::value;


    U32 loopStartIdx = regRowIdxPiv;
    if constexpr (dataIsVectorData)
        loopStartIdx += iteration * numColRegisters;
    U32 regDiff = regRowIdxSwp - regRowIdxPiv;

    for (U32 i = loopStartIdx; i < matData.size(); i += numColRegisters)
        Exchange<_regElmIdxPiv, _regElmIdxSwp>(matData[i], matData[i + regDiff]);


    if constexpr (dataIsVectorData)
        Exchange<_regElmIdxPiv, _regElmIdxSwp>(vecPermData[regRowIdxPiv], vecPermData[regRowIdxPiv + regDiff]);
    else
        vecPermData.AddPermutation(regRowIdxPiv, regRowIdxPiv + regDiff,
                                   &PermutationFunction<_regElmIdxPiv, _regElmIdxSwp, false>);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
template <U32 _regElmIdxPiv, U32 _regElmIdxSwp, typename _typeVecPerm>
inline void PivotDenseSSE<_registerType, _size>::SwapRowPivotLoopSameReg(U32 iteration, U32 regRowIdxPiv,
                                                                         MatrixDataArray& matData,
                                                                         _typeVecPerm& vecPermData)
{
    using namespace GDL::simd;


    if constexpr (_regElmIdxPiv < _regElmIdxSwp)
    {
        constexpr bool dataIsVectorData = std::is_same<_typeVecPerm, VectorDataArray>::value;


        U32 loopStartIdx = regRowIdxPiv;
        if constexpr (dataIsVectorData)
            loopStartIdx += iteration * numColRegisters;

        for (U32 i = loopStartIdx; i < matData.size(); i += numColRegisters)
            matData[i] = Swap<_regElmIdxPiv, _regElmIdxSwp>(matData[i]);


        if constexpr (dataIsVectorData)
            vecPermData[regRowIdxPiv] = Swap<_regElmIdxPiv, _regElmIdxSwp>(vecPermData[regRowIdxPiv]);
        else
            vecPermData.AddPermutation(regRowIdxPiv, regRowIdxPiv,
                                       &PermutationFunction<_regElmIdxPiv, _regElmIdxSwp, true>);
    }
    else
        THROW("Internal index error - Probably singular matrix."); // LCOV_EXCL_LINE
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size>
template <U32 _regElmIdxPiv, bool _sameReg, typename _typeVecPerm>
inline void PivotDenseSSE<_registerType, _size>::SwapRowPivotSwitch(U32 rowIdxSwp, U32 iteration, U32 regRowIdxPiv,
                                                                    U32 regRowIdxSwp, MatrixDataArray& matData,
                                                                    _typeVecPerm& vecPermData)
{
    static_assert(numRegisterValues == 2 || numRegisterValues == 4 || numRegisterValues == 8,
                  "Only registers with 2,4 or 8 values are supported.");

    if constexpr (numRegisterValues == 2)
        switch (rowIdxSwp % numRegisterValues)
        {
        case 0:
            SwapRowPivotLoop<_regElmIdxPiv, 0, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 1:
            SwapRowPivotLoop<_regElmIdxPiv, 1, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        }

    if constexpr (numRegisterValues == 4)
        switch (rowIdxSwp % numRegisterValues)
        {
        case 0:
            SwapRowPivotLoop<_regElmIdxPiv, 0, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 1:
            SwapRowPivotLoop<_regElmIdxPiv, 1, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 2:
            SwapRowPivotLoop<_regElmIdxPiv, 2, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 3:
            SwapRowPivotLoop<_regElmIdxPiv, 3, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        }

    if constexpr (numRegisterValues == 8)
        switch (rowIdxSwp % numRegisterValues)
        {
        case 0:
            SwapRowPivotLoop<_regElmIdxPiv, 0, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 1:
            SwapRowPivotLoop<_regElmIdxPiv, 1, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 2:
            SwapRowPivotLoop<_regElmIdxPiv, 2, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 3:
            SwapRowPivotLoop<_regElmIdxPiv, 3, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 4:
            SwapRowPivotLoop<_regElmIdxPiv, 4, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 5:
            SwapRowPivotLoop<_regElmIdxPiv, 5, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 6:
            SwapRowPivotLoop<_regElmIdxPiv, 6, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        case 7:
            SwapRowPivotLoop<_regElmIdxPiv, 7, _sameReg>(iteration, regRowIdxPiv, regRowIdxSwp, matData, vecPermData);
            break;
        }
}



} // namespace GDL::Solver
