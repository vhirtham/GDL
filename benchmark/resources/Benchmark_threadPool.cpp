#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/time.h"
#include "gdl/base/timer.h"
#include "gdl/resources/cpu/threadPool.h"


#include <array>
#include <iostream>
#include <vector>


using namespace GDL;
using namespace std::chrono_literals;

// Setup %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

constexpr U32 numSubmits = 10000;
constexpr U32 numLoopIterations = 1000000;
constexpr U32 maxNumThreads = 4;

// Helper Functions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void SubmitToThreadpool(ThreadPool<1>& tp, std::vector<U32>& val, std::vector<U32>& res)
{
    for (U32 k = 0; k < res.size(); ++k)
        tp.Submit(
                [](U32 i, std::vector<U32>& result, const std::vector<U32>& value) {
                    for (U32 j = 0; j < value.size(); ++j)
                        result[i] += value[j];
                },
                k, std::ref(res), std::ref(val));
}

Microseconds GetThreadPoolPerformance(ThreadPool<1>& tp, Timer& timer, std::vector<U32>& val)
{
    std::vector<U32> res;
    res.resize(numSubmits, 0);
    timer.Reset();
    SubmitToThreadpool(tp, val, res);
    while (tp.HasTasks())
        std::this_thread::yield();
    Microseconds time = timer.GetElapsedTime<Microseconds>();
    std::cout << tp.GetNumThreads() << " Threads : " << time.count() << "ms" << std::endl;
    std::this_thread::sleep_for(10ms);
    return time;
}



// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int main()
{
    ThreadPool tp(0);
    std::vector<U32> val;
    std::vector<U32> res;
    Timer timer;

    for (U32 i = 0; i < numLoopIterations; ++i)
        val.push_back(i + 1);
    res.resize(numSubmits, 0);

    // Prerun to get the cpus clocked up
    for (U32 i = 0; i < res.size(); ++i)
        for (U32 j = 0; j < val.size(); ++j)
            res[i] += val[j];

    timer.Reset();
    for (U32 i = 0; i < res.size(); ++i)
        for (U32 j = 0; j < val.size(); ++j)
            res[i] += val[j];
    Microseconds timeSingle = timer.GetElapsedTime<Microseconds>();
    std::cout << "Single : " << timeSingle.count() << "ms" << std::endl;

    timer.Reset();
    SubmitToThreadpool(tp, val, res);
    Microseconds timeSubmission = timer.GetElapsedTime<Microseconds>();
    std::cout << "Submission : " << timeSubmission.count() << "ms" << std::endl;

    tp.StartThreads(1);
    while (tp.HasTasks())
        std::this_thread::yield();
    tp.CloseAllThreads();

    std::array<Microseconds, maxNumThreads> timeThreadpool;
    std::array<F32, maxNumThreads> speedupThreadpool;

    for (U32 i = 0; i < timeThreadpool.size(); ++i)
    {
        tp.StartThreads(1);
        std::this_thread::sleep_for(10ms);
        timeThreadpool[i] = GetThreadPoolPerformance(tp, timer, val);
        speedupThreadpool[i] = static_cast<F32>(timeSingle.count()) / static_cast<F32>(timeThreadpool[i].count());
    }

    std::cout << std::endl << "Speedup" << std::endl << "-------" << std::endl;
    for (U32 i = 0; i < speedupThreadpool.size(); ++i)
        std::cout << i + 1 << " Threads : " << speedupThreadpool[i] << std::endl;
}
