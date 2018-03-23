#pragma once

#include <GL/glew.h>

namespace GDL
{
class Uniform
{
    GLuint mLocation;
    GLuint mSubsequentArrayLength;
    GLenum mType;

public:
    Uniform() = delete;
    Uniform(const Uniform&) = default;
    Uniform(Uniform&&) = default;
    Uniform& operator=(const Uniform&) = default;
    Uniform& operator=(Uniform&&) = default;
    ~Uniform() = default;

    //! @brief Ctor
    //! @param location: The uniforms location
    //! @param type: The uniforms type enum
    //! @param subsequentArrayLength: Number of subsequent array elements (1 for non array types or last elements)
    Uniform(GLint location, GLenum type, GLint subsequentArrayLength)
        : mLocation(location)
        , mSubsequentArrayLength(subsequentArrayLength)
        , mType(type)
    {
    }


    //! @brief Gets the uniforms type enum
    GLenum GetType() const
    {
        return mType;
    }

    //! @brief Gets the uniforms location
    GLuint GetLocation() const
    {
        return mLocation;
    }

    //! @brief Gets the number of subsequent array elements (1 for non array types or last elements)
    GLuint GetSubsequentArrayLength() const
    {
        return mSubsequentArrayLength;
    }
};
}
