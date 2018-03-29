#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"
#include "gdl/rendering/openGL/uniformBlock.h"

#include <GL/glew.h>

#include <cassert>
#include <map>
#include <string>
#include <vector>

namespace GDL
{
class UniformBufferObject
{

    GLuint mHandle;
    GLuint mSize;
    GLint mBindingPoint;

    std::map<std::string, UniformBlockVariable> mVariables;

public:
    UniformBufferObject() = delete;
    UniformBufferObject(const UniformBufferObject&) = delete;
    UniformBufferObject(UniformBufferObject&&) = delete;
    UniformBufferObject& operator=(const UniformBufferObject&) = delete;
    UniformBufferObject& operator=(UniformBufferObject&&) = delete;

    //! @brief Constructs a uniform buffer object from a referenced uniform block and a fitting buffer
    //! @param uniformBlock: A uniform block that contains the structure that should be matched
    //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for glNamedBufferData)
    //! @param buffer: Vector of U8 containing the byte data that should be used for initialization
    UniformBufferObject(const UniformBlock& uniformBlock, GLenum usage, const std::vector<U8>& buffer)
        : mSize(uniformBlock.GetSize())
        , mBindingPoint(uniformBlock.GetBindingPoint())
        , mVariables(uniformBlock.GetVariables())
    {
        Initialize(buffer, usage);
    }

    //! @brief Constructs a uniform buffer object from a referenced uniform block
    //! @param uniformBlock: A uniform block that contains the structure that should be matched
    //! //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for
    //! glNamedBufferData)
    UniformBufferObject(const UniformBlock& uniformBlock, GLenum usage)
        : mSize(uniformBlock.GetSize())
        , mBindingPoint(uniformBlock.GetBindingPoint())
        , mVariables(uniformBlock.GetVariables())
    {
        Initialize(std::vector<U8>(mSize, 0.), usage);
    }

    //! @brief Destructor that deletes the buffer object
    ~UniformBufferObject()
    {
        glDeleteBuffers(1, &mHandle);
    }

    //! @brief Gets the handle of the uniform buffer object
    //! @return Handle of the uniform buffer object
    GLuint GetHandle() const
    {
        return mHandle;
    }

    //! @brief Gets the binding point of the uniform buffer object
    //! @return Binding point of the uniform buffer object
    GLuint GetBindingPoint() const
    {
        return mBindingPoint;
    }

    //! @brief Gets the number of variables of the uniform buffer object
    //! @return Number of variables of the uniform buffer object
    GLuint GetNumVariables() const
    {
        return mVariables.size();
    }

    //! @brief Gets the size of the uniform buffer object
    //! @return Size of the uniform buffer object
    GLuint GetSize() const
    {
        return mSize;
    }

    //! @brief Checks if the uniform buffer objects data layout is compatible to a uniform block
    //! @param uniformBlock: Uniform block that should be checked
    //! @return true/false
    bool CheckUniformBlockCompatibility(const UniformBlock& uniformBlock) const
    {
        if (mSize != uniformBlock.GetSize() || mVariables.size() != uniformBlock.GetNumVariables())
            return false;

        for (const auto& variableBlock : uniformBlock.GetVariables())
        {
            const auto& variableBuffer = mVariables.find(variableBlock.first);
            if (variableBuffer == mVariables.end() ||
                !(variableBuffer->second.CheckEqualDataStructure(variableBlock.second)))
                return false;
        }

        return true;
    }

private:
    //! @brief Initializes the uniform buffer object
    //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for glNamedBufferData)
    void Initialize(const std::vector<U8>& buffer, GLenum usage)
    {
        if (buffer.size() != mSize)
            throw Exception(__PRETTY_FUNCTION__,
                            "Dimension of buffer which is provided for initialization does not fit!");
        glGenBuffers(1, &mHandle);
        glNamedBufferData(mHandle, buffer.size(), buffer.data(), usage);
        glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mHandle);
    }
};
}
