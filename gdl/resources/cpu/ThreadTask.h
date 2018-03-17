#pragma once

#include "gdl/resources/cpu/ThreadTaskBase.h"

namespace GDL
{
//! @brief This class stores a callable object that should be executed when the corresponding member function is called.
//! Due to the template, any callable object (functors, std::functions) can be used.
//! @tparam F: Callable object
template <typename F>
class ThreadTask : public ThreadTaskBase
{
    F mFunction; //!< Stored callable object

public:
    //! @brief Constructor
    ThreadTask() = delete;

    //! @brief Copy constructor
    //! @param other: Object that should be copied
    ThreadTask(const ThreadTask& other) = delete;

    //! @brief move constructor
    //! @param other: Object that should be moved
    ThreadTask(ThreadTask&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    ThreadTask& operator=(const ThreadTask& other) = delete;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    ThreadTask& operator=(ThreadTask&& other) = default;

    //! @brief Destructor
    ~ThreadTask() = default;

    //! @brief Constructor which moves the provided callable object to the tasks internal variable
    //! @param function: Callable object that should be executed by the task
    ThreadTask(F&& function)
        : mFunction(std::move(function))
    {
    }



    //! @brief Executes the stored callable object.
    virtual void execute() override
    {
        mFunction();
    }
};

} // namespace GDL
