#pragma once


namespace GDL
{
class ThreadPool
{

public:
    //! @brief Constructor
    ThreadPool() = default;
    //! @brief Copy constructor
    ThreadPool(const ThreadPool& other) = delete;
    //! @brief Move constructor
    ThreadPool(ThreadPool&& other) = delete;
    //! @brief Copy assignment operator
    ThreadPool& operator=(const ThreadPool& other) = delete;
    //! @brief Move assignment operator
    ThreadPool& operator=(ThreadPool&& other) = delete;
    //! @brief Destructor
    ~ThreadPool() = default;
};


} // namespace GDL
