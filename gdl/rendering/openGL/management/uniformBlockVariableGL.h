#pragma once

#include <GL/glew.h>

namespace GDL
{
namespace OpenGL
{
class UniformBlockVariable
{
    GLuint mIndex;
    GLuint mOffset;
    GLuint mArraySize;
    GLenum mType;


public:
    UniformBlockVariable() = delete;
    UniformBlockVariable(const UniformBlockVariable&) = default;
    UniformBlockVariable(UniformBlockVariable&&) = default;
    UniformBlockVariable& operator=(const UniformBlockVariable&) = default;
    UniformBlockVariable& operator=(UniformBlockVariable&&) = default;
    ~UniformBlockVariable() = default;

    //! @brief ctor
    //! @param index: Index of the uniform block variable
    //! @param offset: The uniforms offset inside the uniform block
    //! @param type: The uniforms type enum
    //! @param arraySize: Number of array elements until the arrays end including the current element (1 for non array
    //! types or last elements)
    UniformBlockVariable(GLuint index, GLuint offset, GLenum type, GLuint arraySize)
        : mIndex(index)
        , mOffset(offset)
        , mArraySize(arraySize)
        , mType(type)
    {
    }


    //! @brief Checks if two uniform block variables have the same data structure
    //! @param other: Uniform block variable that should be compared
    //! @return true/false
    bool CheckEqualDataStructure(const UniformBlockVariable& other) const
    {
        return mOffset == other.mOffset && mArraySize == other.mArraySize && mType == other.mType;
    }

    //! @brief Gets the index of the uniform block variable
    //! @return Index of the uniform block variable
    GLuint GetIndex() const
    {
        return mIndex;
    }

    //! @brief Gets the offset of the uniform block variable inside the uniform block
    //! @return Offset of the uniform block variable
    GLuint GetOffset() const
    {
        return mOffset;
    }

    //! @brief Gets the type enum of the uniform block variable
    //! @return Type enum of the uniform block variable
    GLenum GetType() const
    {
        return mType;
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
}
