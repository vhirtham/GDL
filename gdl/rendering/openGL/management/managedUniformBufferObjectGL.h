#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/rendering/openGL/management/uniformBlockGL.h"
#include "gdl/rendering/openGL/core/uniformBufferObjectGL.h"

#include <GL/glew.h>

#include <map>
#include <string>
#include <vector>

namespace GDL
{
namespace OpenGL
{
//! @brief Class that manages a uniform buffer object with predifined data structure
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

    //! @brief Gets a variable
    //! @param variableName: Name of the variable
    //! @return Requested variable
    const UniformBlockVariable& GetVariable(std::string variableName) const;

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

    //! @brief Writes data to the uniform buffer object starting at the given offset
    //! @tparam _dataType: Data type of the provided data
    //! @param data: The data that should be written to the uniform buffer object
    //! @param offset: Location inside the buffer where the data should be written
    template <typename _dataType>
    void SetData(_dataType data, GLint offset = 0) const
    {
        mUBO.SetData<_dataType>(data, offset);
    }

    //! @brief Writes data to the uniform buffer object starting at the position of a given variable
    //! @tparam _dataType: Data type of the provided data
    //! @param variableName: Name of the variable from which the write offset is determined
    //! @param data: The data that should be written to the uniform buffer object
    template <typename _dataType>
    void SetData(std::string variableName, _dataType data) const
    {
        SetData<_dataType>(data, GetVariable(variableName).GetOffset());
    }

    //! @brief Checks if the uniform buffer objects data layout is compatible to a uniform block
    //! @param uniformBlock: Uniform block that should be checked
    //! @return true/false
    bool CheckUniformBlockCompatibility(const UniformBlock& uniformBlock) const;

private:
    //! @Initializes the manged UBO with data from a uniform block
    void Initialize(const UniformBlock& uniformBlock);
};
} // namespace OpenGL
} // namespace GDL
