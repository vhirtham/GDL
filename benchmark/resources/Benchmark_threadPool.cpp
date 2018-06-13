#include "gdl/GDLTypedefs.h"
#include "gdl/base/timer.h"
#include "gdl/resources/cpu/threadPool.h"


#include <array>
#include <iostream>
#include <vector>


using namespace GDL;

// Setup %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

constexpr U32 numSubmits = 10000;
constexpr U32 numLoopIterations = 1000000;
constexpr U32 maxNumThreads = 4;

// Helper Functions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void SubmitToThreadpool(ThreadPool<1>& tp, std::vector<U32>& val, std::vector<U32>& res)
{
    for (U32 i = 0; i < res.size(); ++i)
        tp.Submit(
                [](U32 i, std::vector<U32>& res, const std::vector<U32>& val) {
                    for (U32 j = 0; j < val.size(); ++j)
                        res[i] += val[j];
                },
                i, std::ref(res), std::ref(val));
}

U32 GetThreadPoolPerformance(ThreadPool<1>& tp, Timer& timer, std::vector<U32>& val)
{
    std::vector<U32> res;
    res.resize(numSubmits, 0);
    timer.Reset();
    SubmitToThreadpool(tp, val, res);
    while (tp.HasTasks())
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    U32 time = timer.GetMilliseconds();
    std::cout << tp.GetNumThreads() << " Threads : " << time << "ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
    U32 timeSingle = timer.GetMilliseconds();
    std::cout << "Single : " << timeSingle << "ms" << std::endl;

    timer.Reset();
    SubmitToThreadpool(tp, val, res);
    U32 timeSubmission = timer.GetMilliseconds();
    std::cout << "Submission : " << timeSubmission << "ms" << std::endl;

    tp.StartThreads(1);
    while (tp.HasTasks())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    tp.CloseAllThreads();

    std::array<U32, maxNumThreads> timeThreadpool;
    std::array<F32, maxNumThreads> speedupThreadpool;

    for (U32 i = 0; i < timeThreadpool.size(); ++i)
    {
        tp.StartThreads(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        timeThreadpool[i] = GetThreadPoolPerformance(tp, timer, val);
        speedupThreadpool[i] = static_cast<F32>(timeSingle) / static_cast<F32>(timeThreadpool[i]);
    }

    std::cout << std::endl << "Speedup" << std::endl << "-------" << std::endl;
    for (U32 i = 0; i < speedupThreadpool.size(); ++i)
        std::cout << i + 1 << " Threads : " << speedupThreadpool[i] << std::endl;
}
