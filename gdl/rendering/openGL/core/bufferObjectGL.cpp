#include "gdl/rendering/openGL/core/bufferObjectGL.h"

#include <cassert>

namespace GDL
{
namespace OpenGL
{



BufferObject::BufferObject(GLuint size, GLenum usage)
    : mSize(size)
    , mUsage(usage)
{
    const std::vector<U8> buffer(size, 0);
    Initialize(buffer, usage);
}



template <typename _dataType>
BufferObject::BufferObject(const std::vector<_dataType>& bufferData, GLenum usage)
    : mSize(bufferData.size())
    , mUsage(usage)
{
    Initialize(bufferData, usage);
}
// explicit instantiation
template BufferObject::BufferObject(const std::vector<U8>& bufferData, GLenum usage);
template BufferObject::BufferObject(const std::vector<F32>& bufferData, GLenum usage);



BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &mHandle);
}



template <>
void BufferObject::SetData<std::vector<U8>>(std::vector<U8> data, GLint offset) const
{
    assert(static_cast<GLsizei>(offset + data.size() * sizeof(U8)) <= mSize);
    glNamedBufferSubData(mHandle, offset, data.size() * sizeof(U8), data.data());
}



template <>
void BufferObject::SetData<std::vector<F32>>(std::vector<F32> data, GLint offset) const
{
    assert(static_cast<GLsizei>(offset + data.size() * sizeof(F32)) <= mSize);
    glNamedBufferSubData(mHandle, offset, data.size() * sizeof(F32), data.data());
}



template <typename _dataType>
void BufferObject::Initialize(const std::vector<_dataType>& bufferData, GLenum usage)
{
    glCreateBuffers(1, &mHandle);
    glNamedBufferData(mHandle, bufferData.size() * sizeof(_dataType), bufferData.data(), usage);
    assert(mHandle > 0);
    assert(glGetError() == GL_NO_ERROR);
}


} // namespace OpenGL
} // namespace GDL
