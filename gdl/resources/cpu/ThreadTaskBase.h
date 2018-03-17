#pragma once


//! @brief This is the base class to the tasks used by the thread pool.
//! @remark Because ThreadTask is a template class, a base class is needed to store all resulting instances in the same
//! queue.
namespace GDL
{
class ThreadTaskBase
{
public:
    //! @brief Constructor
    ThreadTaskBase() = default;

    //! @brief Copy constructor
    //! @param other: Object that should be copied
    ThreadTaskBase(const ThreadTaskBase& other) = delete;

    //! @brief move constructor
    //! @param other: Object that should be moved
    ThreadTaskBase(ThreadTaskBase&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    ThreadTaskBase& operator=(const ThreadTaskBase& other) = delete;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    ThreadTaskBase& operator=(ThreadTaskBase&& other) = default;

    //! @brief Destructor
    ~ThreadTaskBase() = default;

    //! @brief Executes the stored callable object.
    virtual void execute() = 0;
};
} // namespace GDL
