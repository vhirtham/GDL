#pragma once

#include "gdl/math/solver/internal/permutationDataSerial.h"

namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline PermutationData<_type, _size, _pivot>::PermutationData()
{
    for (U32 i = 0; i < _size; ++i)
        mRowPermutation[i] = i;
}

} // namespace GDL::Solver
