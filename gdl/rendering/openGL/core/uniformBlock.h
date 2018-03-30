#pragma once

#include "gdl/base/Exception.h"
#include "gdl/rendering/openGL/core/uniformBlockVariable.h"

#include <GL/glew.h>

#include <map>
#include <string>

namespace GDL
{
class UniformBlock
{
    friend class ProgramDataManager;

    GLuint mIndex;
    GLuint mBindingPoint;
    GLuint mSize;

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
    //! @param size: Size of the whole uniform block
    UniformBlock(GLuint index, GLint bindingPoint, GLint size)
        : mIndex(index)
        , mBindingPoint(bindingPoint)
        , mSize(size)
    {
    }

    //! @brief ctor
    //! @param index: Index of the uniform block
    //! @param bindingPoint: the UBs binding point
    //! @param size: Size of the whole uniform block
    //! @param variables: Map that contains the uniform blocks members
    UniformBlock(GLuint index, GLint bindingPoint, GLint size,
                 const std::map<std::string, UniformBlockVariable>& variables)
        : mIndex(index)
        , mBindingPoint(bindingPoint)
        , mSize(size)
        , mVariables(variables)
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
        return mVariables.size();
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

    //! @brief Gets a constant reference to the internal variable container
    //! @return Constant reference to the internal variable container
    const std::map<std::string, UniformBlockVariable>& GetVariables() const
    {
        return mVariables;
    }
};
}
