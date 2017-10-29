#pragma once



#include <future>

namespace GDL
{
template <typename T>
class TaskFuture
{
    std::future<T> mFuture;

public:
    //! @brief Constructor
    TaskFuture() = delete;

    //! @brief Copy constructor
    //! @param other: Object that should be copied
    TaskFuture(const TaskFuture& rhs) = delete;

    //! @brief move constructor
    //! @param other: Object that should be moved
    TaskFuture(TaskFuture&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    TaskFuture& operator=(const TaskFuture& rhs) = delete;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    TaskFuture& operator=(TaskFuture&& other) = default;

    //! @brief Destructor
    ~TaskFuture(void);

    //! @brief Constructor that takes over a std::future
    //! @param future: Future that should be owned by the class
    TaskFuture(std::future<T>&& future);



    //! @brief Gets the future
    //! @return Future
    auto get(void);
};

template <typename T>
TaskFuture<T>::~TaskFuture()
{
    if (mFuture.valid())
    {
        mFuture.get();
    }
}

template <typename T>
TaskFuture<T>::TaskFuture(std::future<T>&& future)
    : mFuture{std::move(future)}
{
}

template <typename T>
auto TaskFuture<T>::get()
{
    return mFuture.get();
}
}
