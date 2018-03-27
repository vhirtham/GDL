#pragma once

#include "gdl/rendering/openGL/uniformBlockVariable.h"

#include <GL/glew.h>

#include <map>
#include <string>

namespace GDL
{
class UniformBlock
{
    friend class ProgramGL;

    GLuint mIndex;
    GLint mBindingPoint;
    GLint mNumVariables;
    GLint mSize;

    std::map<std::string, UniformBlockVariable> mVariables;

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

    //! @brief Gets the index of the uniform block
    //! @return Index of the uniform block
    GLuint GetIndex() const
    {
        return mIndex;
    }

    //! @brief Gets the binding point of the uniform block
    //! @return Binding point of the uniform block
    GLuint GetBindingPoint() const
    {
        return mBindingPoint;
    }

    //! @brief Gets the number of variables of the uniform block
    //! @return Number of variables of the uniform block
    GLuint GetNumVariables() const
    {
        return mNumVariables;
    }

    //! @brief Gets the size of the uniform block
    //! @return Size of the uniform block
    GLuint GetSize() const
    {
        return mSize;
    }

    //! @brief Gets a variable
    //! @param variableName: Name of the variable
    //! @return Variable
    const UniformBlockVariable& GetVariable(std::string variableName) const
    {
        auto iterator = mVariables.find(variableName);
        if (iterator == mVariables.end())
            throw Exception(__PRETTY_FUNCTION__,
                            "Did not find variable \"" + variableName +
                                    "\". GLSL compiler optimization might be the reason.");
        return iterator->second;
    }
};
}
