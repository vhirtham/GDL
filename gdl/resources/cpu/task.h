#pragma once

#include "gdl/resources/cpu/taskBase.h"

#include <algorithm>

namespace GDL
{
//! @brief This class stores a callable object that should be executed when the corresponding member function is called.
//! Due to the template, any callable object (functors, std::functions) can be used.
//! @tparam _function: Callable object type
template <typename _function>
class Task : public TaskBase
{
    _function mFunction; //!< Stored callable object

public:
    Task() = delete;
    Task(const Task& other) = delete;
    Task(Task&& other) = default;
    Task& operator=(const Task& other) = delete;
    Task& operator=(Task&& other) = default;
    ~Task() = default;

    //! @brief Constructor which moves the provided callable object to the tasks internal variable
    //! @param function: Callable object that should be executed by the task
    Task(_function&& function)
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
