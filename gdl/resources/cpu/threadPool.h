#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/resources/cpu/thread.h"
#include "gdl/resources/cpu/task.h"
#include "gdl/resources/cpu/taskBase.h"
#include "gdl/resources/cpu/threadPoolQueue.h"

#include <deque>
#include <functional>
#include <mutex>

#include <iostream>

namespace GDL
{

class ThreadPoolNEW
{
    friend class ThreadNEW;

    mutable std::mutex mMutexThreads;
    std::deque<ThreadNEW> mThreads;
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

    template <typename F, typename... Args>
    void submit(F&& func, Args&&... args);
};


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
void ThreadPoolNEW::submit(F&& func, Args&&... args)
{
    using ResultType = std::result_of_t<decltype(std::bind(std::forward<F>(func), std::forward<Args>(args)...))()>;
    using TaskType = Task<decltype(std::bind(std::forward<F>(func), std::forward<Args>(args)...))>;

    static_assert(is_void<ResultType>::value, "Used submit() with non void function!");

    mQueue.Push(std::make_unique<TaskType>(std::bind(std::forward<F>(func), std::forward<Args>(args)...)));
}
}
