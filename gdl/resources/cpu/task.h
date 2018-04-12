#pragma once

#include "gdl/resources/cpu/taskBase.h"

#include <algorithm>

namespace GDL
{
//! @brief This class stores a callable object that should be executed when the corresponding member function is called.
//! Due to the template, any callable object (functors, std::functions) can be used.
//! @tparam F: Callable object
template <typename F>
class Task : public TaskBase
{
    F mFunction; //!< Stored callable object

public:
    //! @brief Constructor
    Task() = delete;

    //! @brief Copy constructor
    //! @param other: Object that should be copied
    Task(const Task& other) = delete;

    //! @brief move constructor
    //! @param other: Object that should be moved
    Task(Task&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    Task& operator=(const Task& other) = delete;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    Task& operator=(Task&& other) = default;

    //! @brief Destructor
    ~Task() = default;

    //! @brief Constructor which moves the provided callable object to the tasks internal variable
    //! @param function: Callable object that should be executed by the task
    Task(F&& function)
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
