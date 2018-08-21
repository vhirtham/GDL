#pragma once

#include <chrono>

namespace GDL
{


using Hours = std::chrono::hours;
using Minutes = std::chrono::minutes;
using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using Nanoseconds = std::chrono::nanoseconds;

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;


//! @brief Returns a time point object wich holds the current time
//! @return Time point object with current time
inline TimePoint CurrentTime()
{
   return std::chrono::high_resolution_clock::now();
}

} // namespace GDL
