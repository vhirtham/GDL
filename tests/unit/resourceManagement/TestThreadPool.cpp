#define BOOST_TEST_MODULE ThreadPool test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>


#include "src/resourceManagement/ThreadPool.h"
#include <chrono>
#include <thread>
#include <unistd.h>
#include <set>
#include <map>
#include <iostream>


using namespace GDL;
using namespace std::chrono_literals;

void test()
{
    std::cout << "start thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "thread wake up " << std::this_thread::get_id() << std::endl;
}


BOOST_AUTO_TEST_CASE(General)
{
    using namespace GDL;


    ThreadPool TP(std::thread::hardware_concurrency());
    std::vector<TaskFuture<void>> v;
    for (std::uint32_t i = 0; i < 21; ++i)
    {
        v.push_back(TP.submit([]() { std::this_thread::sleep_for(std::chrono::seconds(1)); }));
    }
    for (auto& item : v)
    {
        item.get();
    }
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
