#pragma once

#include <GL/glew.h>

namespace GDL
{
class ProgramInput
{
    GLuint mHandle;
    GLenum mType;

public:
    ProgramInput() = delete;
    ProgramInput(const ProgramInput&) = default;
    ProgramInput(ProgramInput&&) = default;
    ProgramInput& operator=(const ProgramInput&) = default;
    ProgramInput& operator=(ProgramInput&&) = default;
    ~ProgramInput() = default;

    //! @brief Ctor
    //! @param handle: The program inputs handle
    //! @param type: The program inputs type enum
    ProgramInput(GLint handle, GLenum type)
        : mHandle(handle)
        , mType(type)
    {
    }


    //! @brief Gets the program inputs type enum
    GLenum GetType() const
    {
        return mType;
    }

    //! @brief Gets the program inputs handle
    GLuint GetHandle() const
    {
        return mHandle;
    }
};
}
