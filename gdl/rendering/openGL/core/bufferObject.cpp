#include "gdl/rendering/openGL/core/bufferObject.h"

#include <cassert>


namespace GDL::OpenGL
{


BufferObject::BufferObject(GLsizei size, GLenum usage)
    : mSize(size)
    , mUsage(usage)
{
    const Vector<U8> buffer(static_cast<U32>(size), 0);
    Initialize(buffer.data());
}


BufferObject::BufferObject(const Vector<F32>& bufferData, GLenum usage)
    : mSize(static_cast<GLsizei>(bufferData.size() * sizeof(F32)))
    , mUsage(usage)
{
    Initialize(bufferData.data());
}



BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &mHandle);
}



GLuint BufferObject::GetHandle() const
{
    return mHandle;
}



GLsizei BufferObject::GetSize() const
{
    return mSize;
}



GLenum BufferObject::GetUsage() const
{
    return mUsage;
}



void BufferObject::SetData(Vector<F32> data, GLint offset)
{
    SetData(data.data(), static_cast<GLsizei>(data.size() * sizeof(F32)), offset);
}



template <typename _dataType>
void BufferObject::Initialize(const _dataType* bufferData)
{
    glCreateBuffers(1, &mHandle);
    glNamedBufferData(mHandle, mSize, bufferData, mUsage);
}



template <typename _dataType>
void BufferObject::SetData(const _dataType* data, GLsizei size, GLint offset)
{
    DEV_EXCEPTION(static_cast<GLsizei>(offset) + size > mSize,
                  "Data size is to big to be written with the requested offset");
    glNamedBufferSubData(mHandle, offset, size, data);
}


} // namespace GDL::OpenGL
