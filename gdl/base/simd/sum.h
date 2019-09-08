#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"

namespace GDL::simd
{

//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
inline __m128 Sum(__m128 source);

//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
inline __m128d Sum(__m128d source);

#ifdef __AVX2__

//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
inline __m256 Sum(__m256 source);

//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
inline __m256d Sum(__m256d source);

#endif //__AVX2__

} // namespace GDL::simd

#include "gdl/base/simd/sum.inl"
