#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <chrono>

namespace GDL
{

class Timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> mReferenceTime;

public:
    //! @brief Default ctor which start the internal clock.
    Timer()
        : mReferenceTime{std::chrono::high_resolution_clock::now()}
    {
    }

    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;
    ~Timer() = default;

    //! @brief Resets the internal clock
    void Reset()
    {
        mReferenceTime = std::chrono::high_resolution_clock::now();
    }

    //! @brief Gets the number of milliseconds that have past
    //! @tparam _ReturnType: Return type of the function
    //! @return The number of milliseconds that have past
    template <typename _ReturnType = U32>
    _ReturnType GetMilliseconds() const
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> currentTime{
                std::chrono::high_resolution_clock::now()};
        std::chrono::duration<_ReturnType, std::milli> duration{
                std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - mReferenceTime)};
        return duration.count();
    }

    //! @brief Gets the number of microseconds that have past
    //! @tparam _ReturnType: Return type of the function
    //! @return The number of microseconds that have past
    template <typename _ReturnType = U32>
    _ReturnType GetMicroseconds() const
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> currentTime{
                std::chrono::high_resolution_clock::now()};
        std::chrono::duration<_ReturnType, std::micro> duration{
                std::chrono::duration_cast<std::chrono::microseconds>(currentTime - mReferenceTime)};
        return duration.count();
    }

    //! @brief Gets the number of nanoseconds that have past
    //! @tparam _ReturnType: Return type of the function
    //! @return The number of nanoseconds that have past
    template <typename _ReturnType = U32>
    _ReturnType GetNanoseconds() const
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> currentTime{
                std::chrono::high_resolution_clock::now()};
        std::chrono::duration<_ReturnType, std::nano> duration{
                std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - mReferenceTime)};
        return duration.count();
    }
};
}
