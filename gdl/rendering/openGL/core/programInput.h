#pragma once

#include <GL/glew.h>

namespace GDL
{
class ProgramInput
{
    GLuint mLocation;
    GLenum mType;

public:
    ProgramInput() = delete;
    ProgramInput(const ProgramInput&) = default;
    ProgramInput(ProgramInput&&) = default;
    ProgramInput& operator=(const ProgramInput&) = default;
    ProgramInput& operator=(ProgramInput&&) = default;
    ~ProgramInput() = default;

    //! @brief Ctor
    //! @param location: The program inputs location
    //! @param type: The program inputs type enum
    ProgramInput(GLint location, GLenum type)
        : mLocation(location)
        , mType(type)
    {
    }


    //! @brief Gets the program inputs type enum
    GLenum GetType() const
    {
        return mType;
    }

    //! @brief Gets the program inputs location
    GLuint GetLocation() const
    {
        return mLocation;
    }
};
}
