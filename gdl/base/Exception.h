#pragma once

#include <string>
#include <stdexcept>


#define EXCEPTION(condition, message)                                                                                  \
    if (condition)                                                                                                     \
    throw GDL::Exception(__PRETTY_FUNCTION__, message)


// Debug exceptions are turned off in release mode like assertions. In contrast to assertions, debug exceptions can be
// tessted to be triggered in unit tests.
#ifndef NDEBUG
#define DEBUG_EXCEPTION(condition, message) EXCEPTION(condition, message)
#else
#define DEBUG_EXCEPTION(condition, message)
#endif


#ifndef NDEVEXCEPTION
#define DEV_EXCEPTION(condition, message) EXCEPTION(condition, message)
#else
#define DEV_EXCEPTION(condition, message)
#endif

namespace GDL
{

//! @brief Stores origion and message of an exception
class Exception : public std::exception
{
    std::string mOrigin; //!< Name of the function that caused the exception
    std::string mMessage; //!< Exception message
    std::string mFullMessage; //!< Exception message including the origin


public:
    //! @brief Constructor
    Exception() = delete;

    //! @brief Copy constructor
    //! @param other: Object that should be copied
    Exception(const Exception& other) = default;

    //! @brief Move constructor
    //! @param other: Object that should be moved
    Exception(Exception&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    Exception& operator=(const Exception& other) = default;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    Exception& operator=(Exception&& other) = default;

    //! @brief Destructor
    ~Exception() = default;

    //! @brief Constructor
    //! @param origin: Name of the function that caused the exception
    //! @param message: Exception message
    Exception(std::string origin, std::string message)
        : mOrigin(origin)
        , mMessage(message)
        , mFullMessage("[" + origin + "] " + message)
    {
    }


    //! @brief Gets the full exception message including the origin
    const std::string& getFullMessage() const
    {
        return mFullMessage;
    }

    //! @brief Gets the message of the exception
    const std::string& getMessage() const
    {
        return mMessage;
    }

    //! @brief Gets the origin of the exception
    const std::string& getOrigin() const
    {
        return mOrigin;
    }


    //! @brief Gets the full exception message including the origin
    virtual const char* what() const noexcept override
    {
        return getFullMessage().c_str();
    }
};
}
