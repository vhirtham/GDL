#pragma once

#include <GL/glew.h>

namespace GDL
{
class Uniform
{
    GLint mLocation;
    GLuint mArraySize;
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
    //! @param arraySize: Number of array elements until the arrays end including the current element (1 for non array
    //! types or last elements)
    Uniform(GLint location, GLenum type, GLint arraySize)
        : mLocation(location)
        , mArraySize(arraySize)
        , mType(type)
    {
    }


    //! @brief Gets the uniforms type enum
    //! @return type enum
    GLenum GetType() const
    {
        return mType;
    }

    //! @brief Gets the uniforms location
    //! @return uniform location
    GLint GetLocation() const
    {
        return mLocation;
    }

    //! @brief Gets the number of array elements until the arrays end including the current element (1 for non array
    //! types or last elements)
    //! @return Number of array elements
    GLuint GetArraySize() const
    {
        return mArraySize;
    }
};
}
