#pragma once

#include <GL/glew.h>

namespace GDL
{
class UniformBlock
{
    GLuint mIndex;
    GLint mBindingPoint;
    GLint mNumVariables;
    GLint mSize;

public:
    UniformBlock() = delete;
    UniformBlock(const UniformBlock&) = default;
    UniformBlock(UniformBlock&&) = default;
    UniformBlock& operator=(const UniformBlock&) = default;
    UniformBlock& operator=(UniformBlock&&) = default;
    ~UniformBlock() = default;

    //! @brief ctor
    //! @param index: Index of the uniform block
    //! @param bindingPoint: the UBs binding point
    //! @param numVariables: number of active variables in the uniform block
    //! @param size: Size of the whole uniform block
    UniformBlock(GLuint index, GLint bindingPoint, GLint numVariables, GLint size)
        : mIndex(index)
        , mBindingPoint(bindingPoint)
        , mNumVariables(numVariables)
        , mSize(size)
    {
    }
};
}
