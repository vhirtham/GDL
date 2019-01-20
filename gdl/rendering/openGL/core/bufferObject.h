#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/vector.h"

#include <GL/glew.h>


namespace GDL::OpenGL
{

//! @brief Wrapper class for OpenGL buffer objects
class BufferObject
{

    GLuint mHandle;
    GLsizei mSize;
    GLenum mUsage;

public:
    BufferObject() = delete;
    BufferObject(const BufferObject&) = delete;
    BufferObject(BufferObject&& other);
    BufferObject& operator=(const BufferObject&) = delete;
    BufferObject& operator=(BufferObject&&) = delete;
    ~BufferObject();

    //! @brief Constructs a buffer object with the given size. All values are set to 0.
    //! @param size: Desired size of the buffer object
    //! @param usage: Usage enum
    BufferObject(GLsizei size, GLenum usage);

    //! @brief Constructs a buffer object with the passed data
    //! @param bufferData: Vector of data that should be used for initialization
    //! @param usage: Usage enum
    BufferObject(const Vector<F32>& bufferData, GLenum usage);

    //! @brief Constructs a buffer object with the passed data
    //! @param bufferData: Vector of data that should be used for initialization
    //! @param usage: Usage enum
    BufferObject(const Vector<U32>& bufferData, GLenum usage);

    //! @brief Binds the buffer to the specified uniform buffer binding point
    //! @param bindingPoint: Uniform buffer binding point
    void BindAsUniformBuffer(GLuint bindingPoint) const;

    //! @brief Gets the handle of the buffer object
    //! @return Handle of the buffer object
    GLuint GetHandle() const;

    //! @brief Gets the size of the buffer object in bytes
    //! @return Size of the buffer object in bytes
    GLsizei GetSize() const;

    //! @brief Gets the usage enum of the buffer object
    //! @return Usage enum of the buffer object
    GLenum GetUsage() const;

    //! @return Returns if the buffer object is bound to the specified uniform buffer binding point
    //! @param bindingPoint: Uniform buffer binding point
    //! @return True / False
    //! @remark This involves querying data from the OpenGL API which might be slow.
    bool IsBoundToUniformBufferBindingPoint(GLuint bindingPoint) const;

    //! Queries the handle of the buffer that is currently bound to the specified uniform buffer binding point
    //! @param bindingPoint: Uniform buffer binding point
    static GLuint QueryUniformBufferBinding(GLuint bindingPoint);

    //! @brief Writes data to the buffer object starting at the given offset
    //! @param data: The data that should be written to the buffer object
    //! @param offset: Location inside the buffer where the data should be written
    template <U32 _size>
    void SetData(const std::array<U8, _size>& data, GLint offset = 0);

    //! @brief Writes data to the buffer object starting at the given offset
    //! @param data: The data that should be written to the buffer object
    //! @param offset: Location inside the buffer where the data should be written
    void SetData(const Vector<U8>& data, GLint offset = 0);

    //! @brief Writes data to the buffer object starting at the given offset
    //! @param data: The data that should be written to the buffer object
    //! @param offset: Location inside the buffer where the data should be written
    void SetData(const Vector<F32>& data, GLint offset = 0);


private:
    //! @brief Initializes the buffer object
    //! @param bufferData: Data that should be used for initialization
    template <typename _dataType>
    void Initialize(const _dataType* bufferData);

    //! @brief Writes data to the buffer object starting at the given offset
    //! @param data: The data that should be written to the buffer object
    //! @param size: Size of the data
    //! @param offset: Location inside the buffer where the data should be written
    template <typename _dataType>
    void SetData(const _dataType* data, GLsizei size, GLint offset);
};



// Template definitions ---------------------------------------------------------------------------

template <U32 _size>
void BufferObject::SetData(const std::array<U8, _size>& data, GLint offset)
{
    SetData(data.data(), static_cast<GLsizei>(data.size()), offset);
}

} // namespace GDL::OpenGL
