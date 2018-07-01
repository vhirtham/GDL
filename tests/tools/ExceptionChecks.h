#include <boost/test/unit_test.hpp>

//! These macros help to write tests for exceptions that can be deactivated. There are to versions for each exception
//! type. The GDL_CHECK_THROW_XXX macro uses BOOST_CHECK_THROW if the exception type is activated and
//! BOOST_CHECK_NO_THROW if not. The GDL_CHECK_THROW_XXX_DISABLE macro removes the command if the exception is
//! deactivated. This might be necessary if a missing exception leaves a tested class in an undefined state.

#ifndef NDEBUG
#define GDL_CHECK_THROW_DEBUG(command, exceptionType) BOOST_CHECK_THROW(command, exceptionType)
#define GDL_CHECK_THROW_DEBUG_DISABLE(command, exceptionType) BOOST_CHECK_THROW(command, exceptionType)
#else
#define GDL_CHECK_THROW_DEBUG(command, exceptionType) BOOST_CHECK_NO_THROW(command)
#define GDL_CHECK_THROW_DEBUG_DISABLE(command, exceptionType)
#endif

#ifndef NDEVEXCEPTION
#define GDL_CHECK_THROW_DEV(command, exceptionType) BOOST_CHECK_THROW(command, exceptionType)
#define GDL_CHECK_THROW_DEV_DISABLE(command, exceptionType) BOOST_CHECK_THROW(command, exceptionType)
#else
#define GDL_CHECK_THROW_DEV(command, exceptionType) BOOST_CHECK_NO_THROW(command)
#define GDL_CHECK_THROW_DEV_DISABLE(command, exceptionType)
#endif
