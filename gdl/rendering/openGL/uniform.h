#pragma once

#include <GL/glew.h>

namespace GDL
{
class Uniform
{
    GLint mLocation;
    GLuint mSubsequentArraySize;
    GLenum mType;
    GLint mBlockIndex;

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
    //! @param subsequentArraySize: Number of subsequent array elements (1 for non array types or last elements)
    //! @param blockIndex: the uniforms block index (-1)
    Uniform(GLint location, GLenum type, GLint subsequentArraySize, GLint blockIndex)
        : mLocation(location)
        , mSubsequentArraySize(subsequentArraySize)
        , mType(type)
        , mBlockIndex(blockIndex)
    {
    }


    //! @brief Gets the block index
    //! @return Block index
    GLint GetBlockIndex() const
    {
        return mBlockIndex;
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

    //! @brief Gets the number of subsequent array elements (1 for non array types or last elements)
    //! @retur Subsequent array size
    GLuint GetSubsequentArraySize() const
    {
        return mSubsequentArraySize;
    }
};
}
