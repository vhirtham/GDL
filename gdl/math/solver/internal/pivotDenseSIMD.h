#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>


namespace GDL::Solver
{



//! @brief Helper class for pivoting strategies that are shared amongst multiple solvers
//! @tparam _registerType: Register type of the system
//! @tparam _size: Size of the system
template <typename _registerType, U32 _size>
class PivotDenseSSE
{
    template <typename, U32, Pivot>
    friend class GaussDenseSSE;
    template <typename, U32, Pivot>
    friend class LUDenseSIMD;



    static constexpr U32 alignment = simd::alignmentBytes<_registerType>;
    static constexpr U32 numRegisterValues = simd::numRegisterValues<_registerType>;
    static constexpr U32 numColRegisters = simd::CalcMinNumArrayRegisters<_registerType>(_size);



    using ValueType = decltype(simd::GetDataType<_registerType>());
    using MatrixDataArray = std::array<_registerType, numColRegisters * _size>;
    using VectorDataArray = std::array<_registerType, numColRegisters>;
    using PermutationFuncPtr = void (*)(_registerType&, _registerType&);



    //! @brief Struct that stores the data of a single permutation
    struct VectorPermutationData
    {
        U32 mRegIdx0 = 0;
        U32 mRegIdx1 = 0;
        PermutationFuncPtr mFuncPtr = nullptr;

        VectorPermutationData() = default;

        //! @brief Constructor
        //! @param idx0: Row index of the first register of the permutation
        //! @param idx1: Row index of the second register of the permutation
        //! @param funcPtr: Function pointer to the permutation function
        inline VectorPermutationData(U32 idx0, U32 idx1, PermutationFuncPtr funcPtr);
    };



    //! @brief Struct that stores all permutations
    struct VectorPermutationDataArray
    {
        std::array<VectorPermutationData, _size - 1> mPermutations;
        U32 mNumPermutations = 0;

        //! @brief Adds a permutation
        //! @param idx0: Row index of the first register of the permutation
        //! @param idx1: Row index of the second register of the permutation
        //! @param funcPtr: Function pointer to the permutation function
        inline void AddPermutation(U32 idx0, U32 idx1, PermutationFuncPtr funcPtr);
    };



    //! @brief Finds the maximum absolute value inside of the column containing the pivot element and returns its index.
    //! Rows above the pivot element are not considered
    //! @tparam _regElmIdxPiv: Register element index of the pivot element
    //! @param iteration: Number of the current iteration
    //! @param regRowIdxPiv: Row index of the register that holds the pivot element
    //! @param matData: Matrix data array (column major ordering)
    //! @return Index of the row containing the maximum absolute value
    template <U32 _regElmIdxPiv>
    inline static U32 FindMaxAbsValueCol(U32 iteration, U32 regRowIdxPiv, const MatrixDataArray& matData);

    //! @brief Performs the partial pivoting step for the given iteration
    //! @tparam _regElmIdxPiv: Register element index of the pivot element
    //! @tparam _typeVecPerm: Data type of additional argument. Must be VecSIMD or VectorPermutationDataArray
    //! @param iteration: Number of the current iteration
    //! @param regRowIdxPiv: Row index of the register that holds the pivot element
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecPermData: Vector data or permutation data
    template <U32 _regElmIdxPiv, typename _typeVecPerm>
    inline static void PartialPivotingStepRegister(U32 iteration, U32 regRowIdxPiv, MatrixDataArray& matData,
                                                   _typeVecPerm& vecPermData);

    //! @brief Swaps two value from the same or different register.
    //! @tparam _regElmIdx0: Register element index of the first value
    //! @tparam _regElmIdx1: Register element index of the second value
    //! @tparam _sameReg: TRUE if both values are located in the same register, FALSE otherwise.
    //! @param reg0: First register.
    //! @param reg1: Second register - Unused if both values are located in the same register
    //! @note This is a helper function that wraps the GDL::simd::Swap and the GDL::simd::Exchange function into a
    //! shared interface. Therefore, both operations can be stored using the same function pointer.
    template <U32 _regElmIdx0, U32 _regElmIdx1, bool _sameReg>
    static inline void PermutationFunction(_registerType& reg0, _registerType& reg1);

    //! @brief Permutes a vector with the passed permutation data
    //! @param vectorData: Vector data
    //! @param permutationData: Permutation data
    static inline void PermuteVector(VectorDataArray& vectorData, const VectorPermutationDataArray& permutationData);

    //! @brief Performs the pivoting step for the given iteration
    //! @tparam _regElmIdxPiv: Register element index of the pivot element
    //! @tparam _pivot: Enum to select the pivoting strategy
    //! @tparam _typeVecPerm: Data type of additional argument. Must be VecSIMD or VectorPermutationDataArray
    //! @param iteration: Number of the current iteration
    //! @param regRowIdxPiv: Row index of the register that holds the pivot element
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecPermData: Vector data or permutation data
    template <U32 _regElmIdxPiv, Pivot _pivot, typename _typeVecPerm>
    inline static void PivotingStepRegister(U32 iteration, U32 regRowIdxPiv, MatrixDataArray& matData,
                                            _typeVecPerm& vecPermData);

    //! @brief Swaps the row of the pivot element with the row of the value that should replace the current pivot
    //! element
    //! @tparam _regElmIdxPiv: Register element index of the pivot element
    //! @tparam _typeVecPerm: Data type of additional argument. Must be VecSIMD or VectorPermutationDataArray
    //! @param rowIdxSwp: Index of the row that will be swapped with the pivot elements row
    //! @param iteration: Number of the current iteration
    //! @param regRowIdxPiv: Row index of the register that holds the pivot element
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecPermData: Vector data or permutation data
    template <U32 _regElmIdxPiv, typename _typeVecPerm>
    inline static void SwapRowPivot(U32 rowIdxSwp, U32 iteration, U32 regRowIdxPiv, MatrixDataArray& matData,
                                    _typeVecPerm& vecPermData);

    //! @brief Loops over all relevant matrix columns to swap the row of the pivot element with the row of the value
    //! that should replace the current pivot element
    //! @tparam _regElmIdxPiv: Register element index of the pivot element
    //! @tparam _regElmIdxSwp: Register element index of the value that should replace the current pivot element
    //! @tparam _sameReg: TRUE if both values are located in the same register, FALSE otherwise.
    //! @tparam _typeVecPerm: Data type of additional argument. Must be VecSIMD or VectorPermutationDataArray
    //! @param iteration: Number of the current iteration
    //! @param regRowIdxPiv: Row index of the register that holds the pivot element
    //! @param regRowIdxSwp: Row index of the register that holds the value that replaces the current pivot element
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecPermData: Vector data or permutation data
    template <U32 _regElmIdxPiv, U32 _regElmIdxSwp, bool _sameReg, typename _typeVecPerm>
    inline static void SwapRowPivotLoop(U32 iteration, U32 regRowIdxPiv, U32 regRowIdxSwp, MatrixDataArray& matData,
                                        _typeVecPerm& vecPermData);

    //! @brief Loops over all relevant matrix columns to swap the row of the pivot element with the row of the value
    //! that should replace the current pivot element. The swapped values of each column need to be inside of different
    //! registers
    //! @tparam _regElmIdxPiv: Register element index of the pivot element
    //! @tparam _regElmIdxSwp: Register element index of the value that should replace the current pivot element
    //! @tparam _typeVecPerm: Data type of additional argument. Must be VecSIMD or VectorPermutationDataArray
    //! @param iteration: Number of the current iteration
    //! @param regRowIdxPiv: Row index of the register that holds the pivot element
    //! @param regRowIdxSwp: Row index of the register that holds the value that replaces the current pivot element
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecPermData: Vector data or permutation data
    template <U32 _regElmIdxPiv, U32 _regElmIdxSwp, typename _typeVecPerm>
    inline static void SwapRowPivotLoopDiffReg(U32 iteration, U32 regRowIdxPiv, U32 regRowIdxSwp,
                                               MatrixDataArray& matData, _typeVecPerm& vecPermData);

    //! @brief Loops over all relevant matrix columns to swap the row of the pivot element with the row of the value
    //! that should replace the current pivot element. The swapped values of each column need to be inside of the same
    //! register
    //! @tparam _regElmIdxPiv: Register element index of the pivot element
    //! @tparam _regElmIdxSwp: Register element index of the value that should replace the current pivot element
    //! @tparam _typeVecPerm: Data type of additional argument. Must be VecSIMD or VectorPermutationDataArray
    //! @param iteration: Number of the current iteration
    //! @param regRowIdxPiv: Row index of the register that holds the pivot element
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecPermData: Vector data or permutation data
    template <U32 _regElmIdxPiv, U32 _regElmIdxSwp, typename _typeVecPerm>
    inline static void SwapRowPivotLoopSameReg(U32 iteration, U32 regRowIdxPiv, MatrixDataArray& matData,
                                               _typeVecPerm& vecPermData);

    //! @brief Selects the correct swap function to swap the row of the pivot element with the row of the value that
    //! should replace the current pivot element. This is necessary since permutations of registers need to be known at
    //! compile time.
    //! @tparam _regElmIdxPiv: Register element index of the pivot element
    //! @tparam _sameReg: TRUE if both values are located in the same register, FALSE otherwise.
    //! @tparam _typeVecPerm: Data type of additional argument. Must be VecSIMD or VectorPermutationDataArray
    //! @param rowIdxSwp: Index of the row that will be swapped with the pivot elements row
    //! @param iteration: Number of the current iteration
    //! @param regRowIdxPiv: Row index of the register that holds the pivot element
    //! @param regRowIdxSwp: Row index of the register that holds the value that replaces the current pivot element
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecPermData: Vector data or permutation data
    template <U32 _regElmIdxPiv, bool _sameReg, typename _typeVecPerm>
    inline static void SwapRowPivotSwitch(U32 rowIdxSwp, U32 iteration, U32 regRowIdxPiv, U32 regRowIdxSwp,
                                          MatrixDataArray& matData, _typeVecPerm& vecPermData);
};


} // namespace GDL::Solver



#include "gdl/math/solver/internal/pivotDenseSIMD.inl"
