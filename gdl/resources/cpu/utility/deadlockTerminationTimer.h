#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/timer.h"

#include <iostream>
#include <thread>


namespace GDL
{


//! @brief This class starts a timer in a separate thread and terminates the program after a specified time if it is not
//! destroyed before. Its purpose is to stop a running multi-threading test if it takes too much time because of a
//! deadlock.
class DeadlockTerminationTimer
{
    U32 mMillisecondsUntilTermination;
    bool mStopTerminationTimer = false;
    Timer mTimer;
    std::thread mThread;


public:
    //! @brief Ctor that takes the number of milliseconds until std::terminate is called
    //! @param millisecondsUntilTermination: Milliseconds until std::terminate is called
    DeadlockTerminationTimer(U32 millisecondsUntilTermination = 30000)
        : mMillisecondsUntilTermination{millisecondsUntilTermination}
        , mStopTerminationTimer{false}
        , mThread{[this]() {
            while (!mStopTerminationTimer)
            {
                if (mTimer.GetMilliseconds() > mMillisecondsUntilTermination)
                {
                    // LCOV_EXCL_START
                    std::cout << "Deadlock timer idle time expired. Possible deadlock. Terminating application..."
                              << std::endl;
                    std::cout.flush();
                    std::terminate();
                    // LCOV_EXCL_END
                }
                // no busy waiting ---> valgrind
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }}
    {
    }

    DeadlockTerminationTimer(const DeadlockTerminationTimer&) = delete;
    DeadlockTerminationTimer(DeadlockTerminationTimer&&) = delete;
    DeadlockTerminationTimer& operator=(const DeadlockTerminationTimer&) = delete;
    DeadlockTerminationTimer& operator=(DeadlockTerminationTimer&&) = delete;

    //! @brief Dtor stops the timer and joins the thread
    ~DeadlockTerminationTimer()
    {
        StopTerminationTimer();
        mThread.join();
    }

    //! @brief Stops the termination timer
    void StopTerminationTimer()
    {
        mStopTerminationTimer = true;
    }
};
}
