#include <boost/test/unit_test.hpp>

#include "base/Exception.h"
#include "resourceManagement/ThreadPool.h"

#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <unistd.h>


using namespace GDL;



//! @brief Calculates the sum of all numbers up to N using the gaussian sum formula
constexpr U32 GaussianSumFormula(U32 n)
{
    return (n * n + n) / 2;
}

//! @brief Fills the thread pools queue with a specified number of tasks to add upp the numbers from 1 to numNumbers
void EnqueueSummationTasks(ThreadPool& TP, std::vector<TaskFuture<U32>>& vFuture, U32 numTasks, U32 numNumbers)
{
    vFuture.reserve(numTasks);
    for (U32 i = 0; i < numTasks; ++i)
    {
        vFuture.push_back(TP.submit(
                [](U32 n) {
                    U32 sum = 0;
                    for (U32 i = 1; i <= n; ++i)
                    {
                        sum += i;
                    }
                    // std::cout << std::this_thread::get_id() << " sum: " << sum << std::endl;
                    return sum;
                },
                numNumbers));
    }
}


//! @brief Adds 100 tasks to the thread pool which should calculate the sum of all numbers up to 10.000. Dynamic
//! adjustment of the number of threads is also tested
BOOST_AUTO_TEST_CASE(DynamicThreadCreation)
{
    using namespace GDL;
    constexpr U32 numNumbers = 10000;
    constexpr U32 numTasks = 500;


    // Create thread pool with one thread
    ThreadPool TP(1);
    BOOST_CHECK_EQUAL(1, TP.getNumThreads());


    // add some tasks
    std::vector<TaskFuture<U32>> vFuture;
    EnqueueSummationTasks(TP, vFuture, numTasks, numNumbers);


    // increase threads while calculation is running
    TP.addThreads(3);
    BOOST_CHECK_EQUAL(4, TP.getNumThreads());
#ifndef NDEBUG
    BOOST_CHECK_EQUAL(4, TP.getNumRunningThreads());
#endif

    // get the results and check them
    std::vector<U32> results(numTasks, 0);
    for (U32 i = 0; i < numTasks; ++i)
    {
        results[i] = vFuture[i].get();
    }
    std::vector<U32> expectedValues(numTasks, GaussianSumFormula(numNumbers));
    BOOST_CHECK_EQUAL_COLLECTIONS(results.begin(), results.end(), expectedValues.begin(), expectedValues.end());


    // Add new tasks to the queue
    vFuture.clear();
    EnqueueSummationTasks(TP, vFuture, numTasks, numNumbers);

    // decrease threads while calculation is running - reduction to zero threads should not be possible!
    TP.killThreads(3);
    BOOST_CHECK_EQUAL(1, TP.getNumThreads());


    // get the results and check them
    results = std::vector<U32>(numTasks, 0);
    for (U32 i = 0; i < numTasks; ++i)
    {
        results[i] = vFuture[i].get();
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(results.begin(), results.end(), expectedValues.begin(), expectedValues.end());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
#ifndef NDEBUG
    BOOST_CHECK_EQUAL(1, TP.getNumRunningThreads());
#endif
}


// template <typename T>
// class ParallelVectorAssembler
//{
//    std::map<std::thread::id, std::vector<T>> mData;
//    ThreadPool& mThreadpool;
//    std::mutex mMutex;
//    std::atomic_int mWorkend = 0;


//    std::vector<T>& GetThreadVector(const std::thread::id threadId)
//    {
//        std::lock_guard<std::mutex> lock(mMutex);
//        auto itVector = mData.find(threadId);
//        if (itVector == mData.end())
//        {
//            return mData.emplace(threadId, std::vector<T>()).first->second;
//        }
//        return itVector->second;
//    }

//    void FinalAssemble()
//    {
//        // if (mWorkend == 500)
//        //     bool hiermussDieConditionVariablerein = true;
//        // else
//        ThreadPool::TaskFuture<void>(mThreadpool.submit([this]() { FinalAssemble(); }));
//        std::cout << "reenqued final assemble task" << std::endl;
//    }

// public:
//    ParallelVectorAssembler(ThreadPool& threadPool)
//        : mThreadpool(threadPool)
//        , mWorkend(0)
//    {
//    }

//    void Assemble(std::function<void(std::vector<T>&)> workFunc)
//    {
//        std::vector<ThreadPool::TaskFuture<void>> v;
//        v.push_back(mThreadpool.submit([this]() { FinalAssemble(); }));
//        for (std::uint32_t i = 0; i < 501; ++i)
//        {
//            v.push_back(mThreadpool.submit([this, workFunc]() {


//                workFunc(GetThreadVector(std::this_thread::get_id()));
//                ++mWorkend;

//            }));
//        }
//        // brauch condition variable
//        // funktion zum setzen selbiger wird in den Threadpool gepackt
//        // ist bedingung nicht erfüllt, packt sich die funktion selbst erneut in den threadpool
//        // am besten die funktion als privates member dieser Klasse machen
//    }
//};


// BOOST_AUTO_TEST_CASE(Parallel_Vector)
//{
//    using namespace GDL;

//    std::cout << "test 2" << std::endl;
//    ThreadPool TP(8);
//    ParallelVectorAssembler<double> PVA(TP);
//    PVA.Assemble([](std::vector<double>& vec) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        vec.push_back(3);
//        std::cout << "Done some work" << std::endl;
//    });

//    int a = 0;
//}
