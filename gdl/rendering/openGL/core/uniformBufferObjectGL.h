#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
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
    ~UniformBufferObject() = default;

    //! @brief Constructs a uniform buffer object with the given size. All values are set to 0.
    //! @param size: Desired size of the UBO
    //! @param usage: Enum that specifies the UBOs usage (have a look at the official reference for glNamedBufferData)
    UniformBufferObject(GLuint size, GLenum usage);

    //! @brief Constructs a buffer object with the passed data
    //! @param bufferData: Vector of data that should be used for initialization
    //! @param usage: Enum that specifies the buffer objects usage (have a look at the official reference for
    //! glNamedBufferData)
    template <typename _dataType>
    UniformBufferObject(const Vector<_dataType>& buffer, GLenum usage)
        : mBufferObject(buffer, usage)
    {
    }



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
    //! @tparam _dataType: Data type of the provided data
    //! @param data: The data that should be written to the uniform buffer object
    //! @param offset: Location inside the buffer where the data should be written
    template <typename _dataType>
    void SetData(_dataType data, GLint offset = 0) const
    {
        mBufferObject.SetData(data, offset);
    }



private:
    //! @brief Checks if the buffer is still bound to its binding point
    //! @return true/false
    bool BindingPointValid() const;
};
} // namespace OpenGL
} // namespace GDL
