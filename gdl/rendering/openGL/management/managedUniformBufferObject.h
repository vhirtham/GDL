#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/rendering/openGL/management/uniformBlock.h"
#include "gdl/rendering/openGL/core/uniformBufferObject.h"

#include <GL/glew.h>

#include <map>
#include <string>
#include <vector>

namespace GDL
{
class ManagedUniformBufferObject
{

    UniformBufferObject mUBO;
    std::map<std::string, UniformBlockVariable> mVariables;

public:
    ManagedUniformBufferObject() = delete;
    ManagedUniformBufferObject(const ManagedUniformBufferObject&) = delete;
    ManagedUniformBufferObject(ManagedUniformBufferObject&&) = delete;
    ManagedUniformBufferObject& operator=(const ManagedUniformBufferObject&) = delete;
    ManagedUniformBufferObject& operator=(ManagedUniformBufferObject&&) = delete;
    ~ManagedUniformBufferObject() = default;

    //! @brief Constructs a uniform buffer object from a referenced uniform block and a fitting buffer
    //! @param uniformBlock: A uniform block that contains the structure that should be matched
    //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for glNamedBufferData)
    //! @param buffer: Vector of U8 containing the byte data that should be used for initialization
    ManagedUniformBufferObject(const UniformBlock& uniformBlock, GLenum usage, const std::vector<U8>& buffer);

    //! @brief Constructs a uniform buffer object from a referenced uniform block
    //! @param uniformBlock: A uniform block that contains the structure that should be matched
    //! //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for
    //! glNamedBufferData)
    ManagedUniformBufferObject(const UniformBlock& uniformBlock, GLenum usage);



    //! @brief Gets the handle of the uniform buffer object
    //! @return Handle of the uniform buffer object
    GLuint GetHandle() const
    {
        return mUBO.GetHandle();
    }

    //! @brief Gets the binding point of the uniform buffer object
    //! @return Binding point of the uniform buffer object
    GLuint GetBindingPoint() const
    {
        return mUBO.GetBindingPoint();
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
        return mUBO.GetSize();
    }

    //! @brief Sets the binding point of the uniform buffer object
    //! @param bindingPoint: New binding point
    void SetBindingPoint(GLint bindingPoint)
    {
        mUBO.SetBindingPoint(bindingPoint);
    }

    //! @brief Checks if the uniform buffer objects data layout is compatible to a uniform block
    //! @param uniformBlock: Uniform block that should be checked
    //! @return true/false
    bool CheckUniformBlockCompatibility(const UniformBlock& uniformBlock) const;

private:
    //! @Initializes the manged UBO with data from a uniform block
    void Initialize(const UniformBlock& uniformBlock);
};
}
