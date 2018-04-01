#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/rendering/openGL/core/bufferObjectGL.h"
#include <GL/glew.h>

#include <vector>

namespace GDL
{
namespace OpenGL
{
class UniformBufferObject
{
    GLint mBindingPoint;
    BufferObject mBufferObject;

public:
    UniformBufferObject() = delete;
    UniformBufferObject(const UniformBufferObject&) = delete;
    UniformBufferObject(UniformBufferObject&&) = delete;
    UniformBufferObject& operator=(const UniformBufferObject&) = delete;
    UniformBufferObject& operator=(UniformBufferObject&&) = delete;

    //! @brief Constructs a uniform buffer object with the given size. All values are set to 0.
    //! @param size: Desired size of the UBO
    //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for glNamedBufferData)
    UniformBufferObject(GLuint size, GLenum usage);

    //! @brief Constructs a uniform buffer object with the data from a fitting buffer
    //! @param buffer: Vector of U8 containing the byte data that should be used for initialization
    //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for glNamedBufferData)
    UniformBufferObject(const std::vector<U8>& buffer, GLenum usage);


    //! @brief Destructor that deletes the buffer object
    ~UniformBufferObject();

    //! @brief Gets the handle of the uniform buffer object
    //! @return Handle of the uniform buffer object
    GLuint GetHandle() const
    {
        return mBufferObject.GetHandle();
    }

    //! @brief Gets the binding point of the uniform buffer object
    //! @return Binding point of the uniform buffer object
    GLuint GetBindingPoint() const
    {
        return mBindingPoint;
    }

    //! @brief Gets the size of the uniform buffer object
    //! @return Size of the uniform buffer object
    GLsizei GetSize() const
    {
        return mBufferObject.GetSize();
    }

    //! @brief Gets the usage enum of the uniform buffer object
    //! @return usage enum of the uniform buffer object
    GLenum GetUsage() const
    {
        return mBufferObject.GetUsage();
    }

    //! @brief Sets the binding point of the uniform buffer object
    //! @param bindingPoint: New binding point
    void SetBindingPoint(GLint bindingPoint);

    //! @brief Writes data to the uniform buffer object starting at the given offset
    //! @tparam TDataType: Data type of the provided data
    //! @param data: The data that should be written to the uniform buffer object
    //! @param offset: Location inside the buffer where the data should be written
    template <typename TDataType>
    void SetData(TDataType data, GLint offset = 0) const
    {
        mBufferObject.SetData<TDataType>(data, offset);
    }



private:
    //! @brief Checks if the buffer is still bound to its binding point
    //! @return true/false
    bool BindingPointValid() const;
};
}
}