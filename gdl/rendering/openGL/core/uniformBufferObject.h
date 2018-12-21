#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/vector.h"
#include "gdl/rendering/openGL/core/bufferObject.h"

#include <GL/glew.h>



namespace GDL::OpenGL
{

class UniformBufferObject
{
    GLuint mBindingPoint = 0;
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
    //! @param usage: Usage enum
    UniformBufferObject(GLsizei size, GLenum usage);

    //! @brief Constructs a uniform buffer object with the passed data
    //! @param bufferData: Vector of data that should be used for initialization
    //! @param usage: Usage enum
    UniformBufferObject(const Vector<F32>& bufferData, GLenum usage);

    //! @brief Gets the binding point of the uniform buffer object
    //! @return Binding point of the uniform buffer object
    GLuint GetBindingPoint() const;

    //! @brief Gets the handle of the uniform buffer object
    //! @return Handle of the uniform buffer object
    GLuint GetHandle() const;

    //! @brief Gets the size of the uniform buffer object
    //! @return Size of the uniform buffer object
    GLsizei GetSize() const;

    //! @brief Gets the usage enum of the uniform buffer object
    //! @return usage enum of the uniform buffer object
    GLenum GetUsage() const;

    //! @brief Sets the binding point of the uniform buffer object
    //! @param bindingPoint: New binding point
    void SetBindingPoint(GLuint bindingPoint);

    //! @brief Writes data to the uniform buffer object starting at the given offset
    //! @param data: The data that should be written to the uniform buffer object
    //! @param offset: Location inside the buffer where the data should be written
    template <U32 _size>
    void SetData(const std::array<U8, _size>& data, GLint offset = 0);

    //! @brief Writes data to the uniform buffer object starting at the given offset
    //! @param data: The data that should be written to the uniform buffer object
    //! @param offset: Location inside the buffer where the data should be written
    void SetData(const Vector<U8>& data, GLint offset = 0);

    //! @brief Writes data to the uniform buffer object starting at the given offset
    //! @param data: The data that should be written to the uniform buffer object
    //! @param offset: Location inside the buffer where the data should be written
    void SetData(const Vector<F32>& data, GLint offset = 0);

private:
    //! @brief Checks if the buffer is still bound to its binding point
    //! @return True / False
    bool IsBindingPointValid() const;
};



// Template definitions ---------------------------------------------------------------------------

template <U32 _size>
void UniformBufferObject::SetData(const std::array<U8, _size>& data, GLint offset)
{
    mBufferObject.SetData(data, offset);
}

} // namespace GDL::OpenGL
