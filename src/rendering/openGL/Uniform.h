#pragma once

#include <GL/gl.h>
#include <string>

namespace GDL
{
class Uniform
{
    GLuint mHandle;
    GLenum mType;

public:
    Uniform() = delete;
    Uniform(const Uniform&) = default;
    Uniform(Uniform&&) = default;
    Uniform& operator=(const Uniform&) = default;
    Uniform& operator=(Uniform&&) = default;
    ~Uniform() = default;

    Uniform(GLint handle, GLenum type)
        : mHandle(handle)
        , mType(type)
    {
    }


    GLenum GetType() const
    {
        return mType;
    }

    GLuint GetHandle() const
    {
        return mHandle;
    }
};
}


