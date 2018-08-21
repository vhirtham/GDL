#pragma once

#include "gdl/base/time.h"

#include <chrono>

namespace GDL
{

//! @brief Class which measures the elapsed time since its construction or since the last reset.
class Timer
{
    TimePoint mReferenceTime;

public:
    //! @brief Default ctor which start the internal clock.
    Timer()
        : mReferenceTime{CurrentTime()}
    {
    }

    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;
    ~Timer() = default;

    //! @brief Restarts the measurement by resetting the internal clock
    void Reset()
    {
        mReferenceTime = CurrentTime();
    }

    //! @brief Returns the elapsed time since the beginning of the measurement
    //! @tparam _temporalResolution: Desired temporal resolution
    //! @return Elapsed time since the beginning of the measurement
    template <typename _temporalResolution>
    _temporalResolution GetElapsedTime()
    {
        return std::chrono::duration_cast<_temporalResolution>(CurrentTime() - mReferenceTime);
    }
};
}
