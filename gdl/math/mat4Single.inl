#pragma once
#include "mat4Single.h"


template <class T>
GDL::mat4Single<T>::mat4Single()
    : mA({{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}})
{
}

template <class T>
GDL::mat4Single<T>::mat4Single(T val)
    : mA({val, 0.0, 0.0, 0.0, 0.0, val, 0.0, 0.0, 0.0, 0.0, val, 0.0, 0.0, 0.0, 0.0, val})
{
}

template <class T>
GDL::mat4Single<T>::mat4Single(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13,
                               T v14, T v15)
    : mA({{v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15}})
{
}

template <class T>
GDL::mat4Single<T> GDL::mat4Single<T>::operator*(const mat4Single<T>& other) const
{
    return mat4Single<T>(mA[0] * other.mA[0] + mA[4] * other.mA[1] + mA[8] * other.mA[2] + mA[12] * other.mA[3],
                         mA[1] * other.mA[0] + mA[5] * other.mA[1] + mA[9] * other.mA[2] + mA[13] * other.mA[3],
                         mA[2] * other.mA[0] + mA[6] * other.mA[1] + mA[10] * other.mA[2] + mA[14] * other.mA[3],
                         mA[3] * other.mA[0] + mA[7] * other.mA[1] + mA[11] * other.mA[2] + mA[15] * other.mA[3],

                         mA[0] * other.mA[4] + mA[4] * other.mA[5] + mA[8] * other.mA[6] + mA[12] * other.mA[7],
                         mA[1] * other.mA[4] + mA[5] * other.mA[5] + mA[9] * other.mA[6] + mA[13] * other.mA[7],
                         mA[2] * other.mA[4] + mA[6] * other.mA[5] + mA[10] * other.mA[6] + mA[14] * other.mA[7],
                         mA[3] * other.mA[4] + mA[7] * other.mA[5] + mA[11] * other.mA[6] + mA[15] * other.mA[7],

                         mA[0] * other.mA[8] + mA[4] * other.mA[9] + mA[8] * other.mA[10] + mA[12] * other.mA[11],
                         mA[1] * other.mA[8] + mA[5] * other.mA[9] + mA[9] * other.mA[10] + mA[13] * other.mA[11],
                         mA[2] * other.mA[8] + mA[6] * other.mA[9] + mA[10] * other.mA[10] + mA[14] * other.mA[11],
                         mA[3] * other.mA[8] + mA[7] * other.mA[9] + mA[11] * other.mA[10] + mA[15] * other.mA[11],

                         mA[0] * other.mA[12] + mA[4] * other.mA[13] + mA[8] * other.mA[14] + mA[12] * other.mA[15],
                         mA[1] * other.mA[12] + mA[5] * other.mA[13] + mA[9] * other.mA[14] + mA[13] * other.mA[15],
                         mA[2] * other.mA[12] + mA[6] * other.mA[13] + mA[10] * other.mA[14] + mA[14] * other.mA[15],
                         mA[3] * other.mA[12] + mA[7] * other.mA[13] + mA[11] * other.mA[14] + mA[15] * other.mA[15]);
}

template <class T>
GDL::mat4Single<T>& GDL::mat4Single<T>::operator+=(const mat4Single<T>& rhs)
{
    for (U32 i = 0; i < 16; ++i)
        mA[i] += rhs.mA[i];
    return *this;
}

template <class T>
T GDL::mat4Single<T>::operator()(const U32 row, const U32 col) const
{
    return mA[row + col * 4];
}

template<class T>
const std::array<T, 16> GDL::mat4Single<T>::Data() const
{
    return mA;
}

#ifndef NDEBUG
namespace GDL
{
template <typename T2>
std::ostream& operator<<(std::ostream& os, const GDL::mat4Single<T2>& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " " << mat(i, 2) << " " << mat(i, 3) << " |" << std::endl;
    return os;
}
}
#endif
