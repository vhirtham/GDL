#pragma once


namespace GDL
{
class TaskBase
{
public:
    TaskBase() = default;
    TaskBase(const TaskBase& other) = delete;
    TaskBase(TaskBase&& other) = default;
    TaskBase& operator=(const TaskBase& other) = delete;
    TaskBase& operator=(TaskBase&& other) = default;
    ~TaskBase() = default;

    //! @brief Executes the stored callable object.
    virtual void execute() = 0;
};
} // namespace GDL
