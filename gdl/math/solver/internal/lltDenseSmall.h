#pragma once



#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>


namespace GDL
{



namespace Solver
{


//! @brief Support class for LLT Cholesky solver.
//! @tparam _type: Data type
//! @tparam _size: Size of the linear system
template <typename _type, U32 _size>
class LLTDenseSmallSerial
{
    static_assert(_size == 3 || _size == 4, "Unsupported system size.");

public:
    //! @brief Class that stores the LLT factorization and the permutations
    class Factorization
    {
        friend class LLTDenseSmallSerial;

        std::array<_type, _size * _size> mLLT;

        //! @brief ctor
        //! @param matrixData: Data of the matrix that should be factorized
        Factorization(const std::array<_type, _size * _size>& matrixData);
    };



    //! @brief Calculates the LLT factorization of a symmetric matrix and returns it
    //! @param matrixData: Data of the matrix that should be factorized
    //! @return LLT factorization
    [[nodiscard]] static inline Factorization Factorize(const std::array<_type, _size * _size>& matrixData);

    //! @brief Solves the symmetric system A * x = r for a given r and the factorization of A
    //! @param factorization: Factorization data
    //! @param r: Data of the right-hand side vector
    //! @return Solution of the symmetric system A * x = r
    [[nodiscard]] static inline std::array<_type, _size> Solve(const Factorization& factorization,
                                                               std::array<_type, _size> r);


private:
    //! @brief Performs the backward substitution of the solution process recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param llt: Data of the LLT decomposition
    //! @param r: Data of the right-hand side vector
    template <U32 _idx = _size - 1>
    static inline void BackwardSubstitution(const std::array<_type, _size * _size>& llt, std::array<_type, _size>& r);

    //! @brief Performs a single factorization step
    //! @tparam _idx: Index of the active row
    //! @param factorization: Factorization data
    template <U32 _idx>
    static inline void FactorizationStep(Factorization& factorization);

    //! @brief Performs the LLT factorization including pivoting recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param factorization: Factorization data
    template <U32 _idx = 0>
    static inline void FactorizeLLT(Factorization& factorization);

    //! @brief Performs the forward substitution of the solution process recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param llt: Data of the LLT decomposition
    //! @param r: Data of the right-hand side vector
    template <U32 _idx = 0>
    static inline void ForwardSubstitution(const std::array<_type, _size * _size>& llt, std::array<_type, _size>& r);
};



//! @brief Support class for LLT Cholesky solver.
//! @tparam _type: Data type
//! @tparam _size: Size of the linear system
template <U32 _size>
class LLTDenseSmallSSE
{
    static_assert(_size == 3 || _size == 4, "Unsupported system size.");

public:
    //! @brief Class that stores the LLT factorization and the permutations
    class Factorization
    {
        friend class LLTDenseSmallSSE;

        std::array<__m128, _size> mLLT;

        //! @brief ctor
        //! @param matrixData: Data of the matrix that should be factorized
        Factorization(const std::array<__m128, _size>& matrixData);
    };



    //! @brief Calculates the LLT factorization of a symmetric matrix and returns it
    //! @param matrixData: Data of the matrix that should be factorized
    //! @return LLT factorization
    [[nodiscard]] static inline Factorization Factorize(const std::array<__m128, _size>& matrixData);

    //! @brief Solves the symmetric system A * x = r for a given r and the factorization of A
    //! @param factorization: Factorization data
    //! @param r: Data of the right-hand side vector
    //! @return Solution of the symmetric system A * x = r
    [[nodiscard]] static inline __m128 Solve(const Factorization& factorization, __m128 r);


private:
    //! @brief Adds the transposed of L to the factorization data
    //! @param llt: Data of the LLT decomposition
    static inline void AddLT(std::array<__m128, _size>& llt);

    //! @brief Performs the backward substitution of the solution process recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param llt: Data of the LLT decomposition
    //! @param r: Data of the right-hand side vector
    template <U32 _idx = _size - 1>
    static inline void BackwardSubstitution(const std::array<__m128, _size>& llt, __m128& r);

    //! @brief Performs a single factorization step
    //! @tparam _idx: Index of the active row
    //! @param factorization: Factorization data
    template <U32 _idx>
    static inline void FactorizationStep(Factorization& factorization);

    //! @brief Performs the LLT factorization including pivoting recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param factorization: Factorization data
    template <U32 _idx = 0>
    static inline void FactorizeLLT(Factorization& factorization);

    //! @brief Performs the forward substitution of the solution process recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param llt: Data of the LLT decomposition
    //! @param r: Data of the right-hand side vector
    template <U32 _idx = 0>
    static inline void ForwardSubstitution(const std::array<__m128, _size>& llt, __m128& r);
};



} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/internal/lltDenseSmall.inl"
