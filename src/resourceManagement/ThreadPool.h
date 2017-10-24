#pragma once

#include "src/resourceManagement/ThreadSafeQueue.h"

#include <atomic>
#include <future>
#include <thread>
#include <utility>
#include <vector>

#include <iostream>

namespace GDL
{
//! @brief Threadpool class manages a specified number of threads.
//! @remark Original implemention can be found here:
//! http://roar11.com/2016/01/a-platform-independent-thread-pool-using-c14/
class ThreadPool
{

    class IThreadTask
    {
    public:
        IThreadTask() = default;
        IThreadTask(const IThreadTask& other) = delete;
        IThreadTask(IThreadTask&& other) = default;
        IThreadTask& operator=(const IThreadTask& other) = delete;
        IThreadTask& operator=(IThreadTask&& other) = default;
        ~IThreadTask() = default;

        virtual void execute() = 0;
    };

    template <typename F>
    class ThreadTask : public IThreadTask
    {
        F mFunction;

    public:
        ThreadTask() = delete;
        ThreadTask(const ThreadTask& other) = delete;
        ThreadTask(ThreadTask&& other) = default;
        ThreadTask& operator=(const ThreadTask& other) = delete;
        ThreadTask& operator=(ThreadTask&& other) = default;
        ~ThreadTask() = default;

        ThreadTask(F&& function)
            : mFunction(std::move(function))
        {
        }

        virtual void execute() override
        {
            mFunction();
        }
    };

    template <typename T>
    class TaskFuture
    {
        std::future<T> mFuture;

    public:
        TaskFuture() = delete;
        TaskFuture(const TaskFuture& rhs) = delete;
        TaskFuture(TaskFuture&& other) = default;
        TaskFuture& operator=(const TaskFuture& rhs) = delete;
        TaskFuture& operator=(TaskFuture&& other) = default;
        ~TaskFuture(void)
        {
            if (mFuture.valid())
            {
                mFuture.get();
            }
        }


        TaskFuture(std::future<T>&& future)
            : mFuture{std::move(future)}
        {
        }



        auto get(void)
        {
            return mFuture.get();
        }
    };

    std::atomic_bool mDone;
    ThreadSafeQueue<std::unique_ptr<IThreadTask>> mWorkQueue;
    std::vector<std::thread> mThreads;


public:
    //! @brief Constructor
    ThreadPool() = delete;
    //! @brief Copy constructor
    ThreadPool(const ThreadPool& other) = delete;
    //! @brief Move constructor
    ThreadPool(ThreadPool&& other) = delete;
    //! @brief Copy assignment operator
    ThreadPool& operator=(const ThreadPool& other) = delete;
    //! @brief Move assignment operator
    ThreadPool& operator=(ThreadPool&& other) = delete;
    //! @brief Destructor
    ~ThreadPool()
    {
        destroy();
    }


    explicit ThreadPool(const std::uint32_t numThreads)
        : mDone(false)
        , mWorkQueue()
        , mThreads()
    {
        try
        {
            for (std::uint32_t i = 0u; i < numThreads; ++i)
            {
                mThreads.emplace_back(&ThreadPool::worker, this, i);
            }
        }
        catch (...)
        {
            destroy();
            throw;
        }
    }

    template <typename F, typename... Args>
    auto submit(F&& func, Args&&... args)
    {
        auto boundTask = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
        using ResultType = std::result_of_t<decltype(boundTask)()>;
        using PackagedTask = std::packaged_task<ResultType()>;
        using TaskType = ThreadTask<PackagedTask>;

        PackagedTask task{std::move(boundTask)};
        TaskFuture<ResultType> result{task.get_future()};
        mWorkQueue.push(std::make_unique<TaskType>(std::move(task)));
        return result;
    }

private:
    void destroy()
    {
        mDone = true;
        mWorkQueue.invalidate();
        for (auto& thread : mThreads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    void blabla()
    {
    }

    void worker(std::uint32_t threadNumber)
    {
        while (!mDone)
        {
            std::unique_ptr<IThreadTask> pTask{nullptr};
            if (mWorkQueue.tryWaitPop(pTask))
            {
                pTask->execute();
                std::cout << " - work done by thread " << threadNumber << std::endl;
            }
        }
    }
};


} // namespace GDL
