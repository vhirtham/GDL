#pragma once

#include "gdl/math/solver/internal/backwardSubstitutionDenseSerial.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"



namespace GDL::Solver
{



template <typename _type, U32 _size, bool _isUnit>
inline void BackwardSubstitutionDenseSerial<_type, _size, _isUnit>::SolveInPlace(const MatrixDataArray& matrixData,
                                                                                 VectorDataArray& rhsData)
{
    for (U32 i = _size; i-- > 0;)
    {
        DEV_EXCEPTION(matrixData[(_size + 1) * i] == ApproxZero<_type>(1, 100),
                      "Can't solve system - Singular matrix or inappropriate pivoting strategy.");

        if constexpr (not _isUnit)
        {
            const U32 pivIdx = (_size + 1) * i;
            rhsData[i] /= matrixData[pivIdx];
        }

        for (U32 j = 0; j < i; ++j)
            rhsData[j] -= rhsData[i] * matrixData[j + i * _size];
    }
}


} // namespace GDL::Solver
