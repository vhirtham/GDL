#pragma once

#include <GL/glew.h>

namespace GDL
{
class Uniform
{
    GLuint mHandle;
    GLuint mSubsequentElementCount;
    GLenum mType;

public:
    Uniform() = delete;
    Uniform(const Uniform&) = default;
    Uniform(Uniform&&) = default;
    Uniform& operator=(const Uniform&) = default;
    Uniform& operator=(Uniform&&) = default;
    ~Uniform() = default;

    //! @brief Ctor
    //! @param handle: The uniforms handle
    //! @param type: The uniforms type enum
    //! @param subsequentElementCount: Number of subsequent array elements (1 for non array types or last elements)
    Uniform(GLint handle, GLenum type, GLint subsequentElementCount)
        : mHandle(handle)
        , mSubsequentElementCount(subsequentElementCount)
        , mType(type)
    {
    }


    //! @brief Gets the uniforms type enum
    GLenum GetType() const
    {
        return mType;
    }

    //! @brief Gets the uniforms handle
    GLuint GetHandle() const
    {
        return mHandle;
    }

    //! @brief Gets the number of subsequent array elements (1 for non array types or last elements)
    GLuint GetSubsequentElementCount() const
    {
        return mSubsequentElementCount;
    }
};
}
