#pragma once

#include "gdl/GDLTypedefs.h"

#include <GL/glew.h>

#include <cassert>
#include <vector>

namespace GDL
{
namespace OpenGL
{

class BufferObject
{

    GLuint mHandle;
    GLsizei mSize;
    GLenum mUsage;

public:
    BufferObject() = delete;
    BufferObject(const BufferObject&) = delete;
    BufferObject(BufferObject&&) = delete;
    BufferObject& operator=(const BufferObject&) = delete;
    BufferObject& operator=(BufferObject&&) = delete;
    ~BufferObject();

    //! @brief Constructs a buffer object with the given size. All values are set to 0.
    //! @param size: Desired size of the buffer object
    //! @param usage: Enum that specifies the buffer objects usage (have a look at the official reference for
    //! glNamedBufferData)
    BufferObject(GLuint size, GLenum usage);

    //! @brief Constructs a buffer object with the passed data
    //! @param bufferData: Vector of data that should be used for initialization
    //! @param usage: Enum that specifies the buffer objects usage (have a look at the official reference for
    //! glNamedBufferData)
    template <typename TDataType>
    explicit BufferObject(const std::vector<TDataType>& bufferData, GLenum usage);

    //! @brief Gets the handle of the buffer object
    //! @return Handle of the buffer object
    GLuint GetHandle() const
    {
        return mHandle;
    }

    //! @brief Gets the size of the buffer object
    //! @return Size of the buffer object
    GLsizei GetSize() const
    {
        return mSize;
    }

    //! @brief Gets the usage enum of the buffer object
    //! @return usage enum of the buffer object
    GLenum GetUsage() const
    {
        return mUsage;
    }

    //! @brief Writes data to the buffer object starting at the given offset
    //! @tparam TDataType: Data type of the provided data
    //! @param data: The data that should be written to the buffer object
    //! @param offset: Location inside the buffer where the data should be written
    template <typename TDataType>
    void SetData(TDataType data, GLint offset = 0) const;

private:
    //! @brief Initializes the buffer object
    //! @param bufferData: Data that should be used for initialization
    //! @param usage: Enum that specifies the buffer objects usage (have a look at the official reference for
    //! glNamedBufferData)
    template <typename TDataType>
    void Initialize(const std::vector<TDataType>& bufferData, GLenum usage);
};
}
}
