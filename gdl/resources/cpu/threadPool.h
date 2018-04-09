#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/resources/cpu/thread.h"
#include "gdl/resources/cpu/task.h"
#include "gdl/resources/cpu/taskBase.h"
#include "gdl/resources/cpu/threadPoolQueue.h"

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>

#include <iostream>

namespace GDL
{

class ThreadPoolNEW
{
    friend class ThreadNEW;



    mutable std::mutex mMutex;
    std::condition_variable mConditionThreads;

    std::deque<ThreadNEW> mThreads;
    std::atomic_bool mClose = false;
    ThreadPoolQueue<std::unique_ptr<TaskBase>> mQueue;

public:
    ThreadPoolNEW() = delete;
    ThreadPoolNEW(const ThreadPoolNEW&) = delete;
    ThreadPoolNEW(ThreadPoolNEW&&) = delete;
    ThreadPoolNEW& operator=(const ThreadPoolNEW&) = delete;
    ThreadPoolNEW& operator=(ThreadPoolNEW&&) = delete;
    ~ThreadPoolNEW();


    explicit ThreadPoolNEW(const U32 numThreads);

    void Deinitialize();

    void GetTask();

    bool HasTasks();

    template <typename F, typename... Args>
    void Submit(F&& func, Args&&... args);

    // ThisThreadWaitFor must be renamed to something that makes clear that the condition
    // is enqued and only checked when the task is processed.
    template <typename F>
    void SubmitThreadWaitCondition(F&& function);

    template <typename F>
    void CheckEnque(ThreadPoolNEW& threadPool, F&& function, std::condition_variable& condition);
};


// http://en.cppreference.com/w/cpp/language/template_specialization
template <typename T> // primary template
struct is_bool : std::false_type
{
};
template <> // explicit specialization for T = void
struct is_bool<bool> : std::true_type
{
};

template <typename F>
void ThreadPoolNEW::SubmitThreadWaitCondition(F&& function)
{
    static_assert(is_bool<std::result_of_t<decltype(function)()>>::value, "Return type moost be bool");

    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    std::condition_variable condition;
    bool condition_met = false;

    CheckEnque(*this,
               [&] {
                   if (function())
                       condition_met = true;
                   return condition_met;
               },
               condition);

    condition.wait(lock, [&] { return condition_met; });
}

template <typename F>
void ThreadPoolNEW::CheckEnque(ThreadPoolNEW& threadPool, F&& function, std::condition_variable& condition)
{
    if (function())
        condition.notify_one();
    else
    {
        Submit([&] { threadPool.CheckEnque(threadPool, function, condition); });
    }
}


// http://en.cppreference.com/w/cpp/language/template_specialization
template <typename T> // primary template
struct is_void : std::false_type
{
};
template <> // explicit specialization for T = void
struct is_void<void> : std::true_type
{
};


template <typename F, typename... Args>
void ThreadPoolNEW::Submit(F&& func, Args&&... args)
{
    // static assertion
    using ResultType = std::result_of_t<decltype(std::bind(std::forward<F>(func), std::forward<Args>(args)...))()>;
    using TaskType = Task<decltype(std::bind(std::forward<F>(func), std::forward<Args>(args)...))>;

    static_assert(is_void<ResultType>::value, "Used submit() with non void function!");
    mQueue.Push(std::make_unique<TaskType>(std::bind(std::forward<F>(func), std::forward<Args>(args)...)));
    mConditionThreads.notify_one();
}
}
