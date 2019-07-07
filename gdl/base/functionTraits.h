#pragma once

//! INFO: This header was inspired by the following stackoverflow topics:
//! https://stackoverflow.com/questions/18695564/deducing-a-function-pointer-return-type
//! https://stackoverflow.com/questions/9065081/how-do-i-get-the-argument-types-of-a-function-pointer-in-a-variadic-template-cla

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <functional>

namespace GDL
{

template <typename _type>
struct FunctionTraits;



//! @brief Struct to extract type information from a function pointer
//! @tparam _returnType: Return type of the function
//! @tparam _args: Function arguments (parameter pack)
template <typename _returnType, typename... _args>
struct FunctionTraits<_returnType (*)(_args...)>
{
    //! @brief Number of function parameters
    static const size_t numArguments = sizeof...(_args);

    //! @brief Return type of the function
    using ReturnType = _returnType;



    //! @brief Helper struct to extract the argument types
    //! @tparam _idx: Index of the argument
    template <size_t _idx>
    struct Argument
    {
        static_assert(_idx < numArguments, "Index exceeds numper of function parameters.");


        //! @brief Type of the argument (including reference and cv-qualifier)
        using Type = typename std::tuple_element<_idx, std::tuple<_args...>>::type;

        //! @brief Value type of the argument (excluding reference and cv-qualifier)
        using ValueType = typename std::remove_cv<typename std::remove_reference<Type>::type>::type;
    };
};



//! @brief Struct to extract type information from a std::function
//! @tparam _returnType: Return type of the function
//! @tparam _args: Function arguments (parameter pack)
template <typename _returnType, typename... _args>
struct FunctionTraits<std::function<_returnType(_args...)>>
{
    //! @brief Number of function parameters
    static const size_t numArguments = sizeof...(_args);

    //! @brief Return type of the function
    using ReturnType = _returnType;



    //! @brief Helper struct to extract the argument types
    //! @tparam _idx: Index of the argument
    template <size_t _idx>
    struct Argument
    {
        static_assert(_idx < numArguments, "Index exceeds numper of function parameters.");


        //! @brief Type of the argument (including reference and cv-qualifier)
        using Type = typename std::tuple_element<_idx, std::tuple<_args...>>::type;

        //! @brief Value type of the argument (excluding reference and cv-qualifier)
        using ValueType = typename std::remove_cv<typename std::remove_reference<Type>::type>::type;
    };
};


} // namespace GDL
