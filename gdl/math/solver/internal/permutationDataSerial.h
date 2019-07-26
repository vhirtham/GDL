#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>


namespace GDL::Solver
{



//! @brief Struct that stores all permutations
template <typename _type, U32 _size,Pivot _pivot>
struct PermutationData
{
    using VectorDataArray = std::array<_type, _size>;

    VectorDataArray mRowPermutation;

    //! @brief Default constructor
    inline PermutationData();
};



//! @brief Template specialization of PermutationData
template <typename _type, U32 _size>
struct PermutationData<_type,_size,Pivot::NONE>
{
};



} // namespace GDL::Solver

#include "gdl/math/solver/internal/permutationDataSerial.inl"
