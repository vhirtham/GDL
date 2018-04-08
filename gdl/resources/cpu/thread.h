#pragma once

#include <atomic>
#include <thread>

namespace GDL
{

class ThreadPoolNEW;

class ThreadNEW
{


    std::atomic_bool mClose;
    std::atomic_bool mIsClosed;
    ThreadPoolNEW& mThreadPool;
    std::thread mThread; // <--- Always last member (initialization problems may occur if not)

public:
    ThreadNEW() = delete;
    ThreadNEW(const ThreadNEW&) = delete;
    ThreadNEW(ThreadNEW&&) = delete;
    ThreadNEW& operator=(const ThreadNEW&) = delete;
    ThreadNEW& operator=(ThreadNEW&&) = delete;
    ~ThreadNEW();

    ThreadNEW(ThreadPoolNEW& threadPool);


    bool IsClosed() const;
    void Run();
};
}
