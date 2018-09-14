#pragma once

#include <atomic>

namespace GDL
{


//! @brief Spinlock for busy waiting
//! @remark https://stackoverflow.com/questions/26583433/c11-implementation-of-spinlock-using-atomic
class SpinLock
{
    std::atomic_flag mLocked = ATOMIC_FLAG_INIT;

public:
    SpinLock() = default;
    SpinLock(const SpinLock&) = delete;
    SpinLock(SpinLock&&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;
    SpinLock& operator=(SpinLock&&) = delete;
    ~SpinLock() = default;



    //! @brief Locks the spinlock
    void lock()
    {
        while (mLocked.test_and_set(std::memory_order_acquire))
            ;
    }


    //! @brief Unlocks the spinlock
    void unlock()
    {
        mLocked.clear(std::memory_order_release);
    }
};

} // namespace GDL
