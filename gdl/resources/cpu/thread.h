#pragma once

#include <thread>
#include <memory>


namespace GDL {


class Thread
{
    std::thread mThread;
public:

    //! Ctor which starts a thread with the provided function
    //! @tparam _Func: Function type
    //! @param function: Function that should be run by the thread
    template <typename _Func>
    Thread(_Func&& function);

    Thread() = delete;
    Thread(const Thread&) = delete;
    Thread(Thread&&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread& operator=(Thread&&) = delete;

    //! @param Dtor which closes the thread
    ~Thread();

    //! @brief Closes the thread (joins if joinable)
    void Close();
};

} // namespace GDL

#include "gdl/resources/cpu/thread.inl"
