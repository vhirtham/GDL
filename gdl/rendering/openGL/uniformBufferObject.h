#pragma once

#include "gdl/GDLTypedefs.h"
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
    GLint mBindingPoint;

    std::vector<U8> mBuffer;
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
    UniformBufferObject(const UniformBlock& uniformBlock, GLenum usage, std::vector<U8>&& buffer)
        : mBindingPoint(uniformBlock.GetBindingPoint())
        , mBuffer(std::move(buffer))
        , mVariables(uniformBlock.mVariables)
    {
        assert(mBuffer.size() == uniformBlock.GetSize());
        Initialize(usage);
    }

    //! @brief Constructs a uniform buffer object from a referenced uniform block
    //! @param uniformBlock: A uniform block that contains the structure that should be matched
    //! //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for glNamedBufferData)
    UniformBufferObject(const UniformBlock& uniformBlock, GLenum usage)
        : mBindingPoint(uniformBlock.GetBindingPoint())
        , mBuffer(uniformBlock.GetSize(),0)
        , mVariables(uniformBlock.mVariables)
    {
        Initialize(usage);
    }

    //! @brief Destructor that deletes the buffer object
    ~UniformBufferObject()
    {
        glDeleteBuffers(1,&mHandle);
    }

private:

    //! @brief Initializes the uniform buffer object
    //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for glNamedBufferData)
    void Initialize(GLenum usage)
    {
        glGenBuffers(1, &mHandle);
        glNamedBufferData(mHandle,mBuffer.size(),mBuffer.data(),usage);
        glBindBufferBase(GL_UNIFORM_BUFFER,mBindingPoint,mHandle);
    }
};
}
