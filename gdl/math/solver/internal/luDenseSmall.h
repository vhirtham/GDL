#pragma once


#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/base/fundamentalTypes.h"

#include "gdl/math/solver/pivot.h"

#include <array>
#include <type_traits>
#include <xmmintrin.h>


namespace GDL
{
class Mat4fAVX;
template <typename>
class Mat3Serial;
template <typename>
class Mat4Serial;
class Mat3fSSE;
class Mat4fSSE;
template <typename, bool>
class Vec3Serial;
template <typename, bool>
class Vec4Serial;
template <bool>
class Vec3fSSE;
template <bool>
class Vec4fSSE;


namespace Solver
{


//! @brief Support class for LU solver.
//! @tparam _type: Data type
//! @tparam _size: Size of the linear system
//! @tparam _pivot: Enum to select pivoting strategy
template <typename _type, U32 _size, Pivot _pivot = Pivot::PARTIAL>
class LUDenseSmallSerial
{
    static_assert(_size == 3 || _size == 4, "Unsupported system size.");

public:
    //! @brief Class that stores the LU factorization and the permutations
    class Factorization
    {
        friend class LUDenseSmallSerial;

        std::array<_type, _size * _size> mLU;
        std::array<_type, 4> mPermutation;

        //! @brief ctor
        //! @param matrixData: Data of the matrix that should be factorized
        Factorization(const std::array<_type, _size * _size>& matrixData);
    };



    //! @brief Calculates the LU factorization and returns it
    //! @param matrixData: Data of the matrix that should be factorized
    //! @return LU factorization
    [[nodiscard]] static inline Factorization Factorize(const std::array<_type, _size * _size>& matrixData);

    //! @brief Solves the system A * x = r for a given r and the factorization of A
    //! @param factorization: Factorization data
    //! @param r: Data of the right-hand side vector
    //! @return Solution of the system A * x = r
    [[nodiscard]] static inline std::array<_type, _size> Solve(const Factorization& factorization,
                                                               std::array<_type, _size> r);


private:
    //! @brief Performs the backward substitution of the solution process recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param lu: Data of the LU decomposition
    //! @param r: Data of the right-hand side vector
    template <U32 _idx = _size - 1>
    static inline void BackwardSubstitution(const std::array<_type, _size * _size>& lu, std::array<_type, _size>& r);

    //! @brief Performs a single factorization step
    //! @tparam _idx: Index of the active row
    //! @param factorization: Factorization data
    template <U32 _idx>
    static inline void FactorizationStep(Factorization& factorization);

    //! @brief Performs the LU factorization including pivoting recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param factorization: Factorization data
    template <U32 _idx = 0>
    static inline void FactorizeLU(Factorization& factorization);

    //! @brief Performs the forward substitution of the solution process recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param lu: Data of the LU decomposition
    //! @param r: Data of the right-hand side vector
    template <U32 _idx = 0>
    static inline void ForwardSubstitution(const std::array<_type, _size * _size>& lu, std::array<_type, _size>& r);

    //! @brief Performs partial pivoting if necessary
    //! @tparam _idx: Index of the active row
    //! @param factorization: Factorization data
    template <U32 _idx>
    static inline void PartialPivot(Factorization& factorization);

    //! @brief Applies the necessary row interchanges of the factorization to the right hand side vector and returns it
    //! @param r: Original right-hand side vector
    //! @param permutation: Permutation data
    //! @return Permuted right-hand side vector
    static inline std::array<_type, _size> RhsPermutation(const std::array<_type, _size>& r,
                                                          const std::array<_type, _size>& permutation);
};



//! @brief Support class for LU solver.
//! @tparam _size: Size of the linear system
//! @tparam _pivot: Enum to select pivoting strategy
template <U32 _size, Pivot _pivot = Pivot::PARTIAL>
class LUDenseSmallSSE
{
    static_assert(_size == 3 || _size == 4, "Unsupported system size.");


public:
    //! @brief Class that stores the LU factorization and the permutations
    class Factorization
    {
        friend class LUDenseSmallSSE;

        std::array<__m128, _size> mLU;
        U32 mPermutation;

        //! @brief ctor
        //! @param matrixData: Data of the matrix that should be factorized
        Factorization(const std::array<__m128, _size>& lu);
    };



    //! @brief Calculates the LU factorization and returns it
    //! @param matrixData: Data of the matrix that should be factorized
    //! @return LU factorization
    [[nodiscard]] static inline Factorization Factorize(const std::array<__m128, _size>& matrixData);

    //! @brief Solves the system A * x = r for a given r and the factorization of A
    //! @param factorization: Factorization data
    //! @param r: Data of the right-hand side vector
    //! @return Solution of the system A * x = r
    [[nodiscard]] static inline __m128 Solve(const Factorization& factorization, __m128 r);


private:
    //! @brief Performs the backward substitution of the solution process recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param lu: Data of the LU decomposition
    //! @param r: Data of the right-hand side vector
    template <U32 _idx = _size - 1>
    static inline void BackwardSubstitution(const std::array<__m128, _size>& lu, __m128& r);

    //! @brief Performs a single factorization step
    //! @tparam _idx: Index of the active row
    //! @param factorization: Factorization data
    template <U32 _idx>
    static inline void FactorizationStep(Factorization& factorization);

    //! @brief Performs the LU factorization including pivoting recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param factorization: Factorization data
    template <U32 _idx = 0>
    static inline void FactorizeLU(Factorization& factorization, __m128& permutation);

    //! @brief Performs the forward substitution of the solution process recursively (template)
    //! @tparam _idx: Current row index. This is used for template recursion and should't be set manually.
    //! @param lu: Data of the LU decomposition
    //! @param r: Data of the right-hand side vector
    template <U32 _idx = 0>
    static inline void ForwardSubstitution(const std::array<__m128, _size>& lu, __m128& r);

    //! @brief Performs partial pivoting if necessary
    //! @tparam _idx: Index of the active row
    //! @param factorization: Factorization data
    //! @param permutation: Register that keeps track of the permutations.
    //! @note The content of the permutation register is turned into a single integer at the end of the factorization
    //! process. This value is then stored in the factorization class
    template <U32 _idx>
    static inline void PartialPivot(Factorization& factorization, __m128& permutation);

    //! @brief Applies the necessary row interchanges of the factorization to the right hand side vector and returns it
    //! @param r: Original right-hand side vector
    //! @param permutation: Permutation data
    //! @return Permuted right-hand side vector
    static inline __m128 RhsPermutation(const __m128& r, U32 permutation);
};



} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/internal/luDenseSmall.inl"
