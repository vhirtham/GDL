#pragma once

#include <string>
#include <stdexcept>



// Macros -------------------------------------------------------------------------------------------------------------

#define EXCEPTION(condition, message)                                                                                  \
    if (condition)                                                                                                     \
    throw GDL::Exception(__PRETTY_FUNCTION__, message)


#define BAD_ALLOC(condition)                                                                                           \
    if (condition)                                                                                                     \
    throw std::bad_alloc()

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



// Class definition ---------------------------------------------------------------------------------------------------

namespace GDL
{

//! @brief Stores origion and message of an exception
class Exception : public std::runtime_error
{

public:
    Exception() = delete;
    Exception(const Exception& other) = default;
    Exception(Exception&& other) = default;
    Exception& operator=(const Exception& other) = default;
    Exception& operator=(Exception&& other) = default;
    virtual ~Exception() = default;

    //! @brief Constructor
    //! @param origin: Name of the function that caused the exception
    //! @param message: Exception message
    Exception(std::string origin, std::string message)
        : std::runtime_error{"[" + origin + "] " + message}
    {
    }
};
}
