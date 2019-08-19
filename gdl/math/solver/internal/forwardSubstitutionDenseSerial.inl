#pragma once

#include "gdl/math/solver/internal/forwardSubstitutionDenseSerial.h"

#include "gdl/base/approx.h"



namespace GDL::Solver
{



template <typename _type, U32 _size, bool _isUnit>
inline void ForwardSubstitutionDenseSerial<_type, _size, _isUnit>::SolveInPlace(const MatrixDataArray& matrixData,
                                                                                VectorDataArray& rhsData)
{
    for (U32 i = 0; i < _size - 1; ++i)
    {
        DEV_EXCEPTION(matrixData[(_size + 1) * i] == ApproxZero<_type>(1, 100),
                      "Can't solve system - Singular matrix.");

        if constexpr (not _isUnit)
        {
            const U32 pivIdx = (_size + 1) * i;
            rhsData[i] /= matrixData[pivIdx];
        }

        for (U32 j = i + 1; j < _size; ++j)
            rhsData[j] -= matrixData[j + i * _size] * rhsData[i];
    }
}


} // namespace GDL::Solver
