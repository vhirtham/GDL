#include "gdl/rendering/openGL/core/bufferObject.h"



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



BufferObject::BufferObject(const Vector<U32>& bufferData, GLenum usage)
    : mSize(static_cast<GLsizei>(bufferData.size() * sizeof(U32)))
    , mUsage(usage)
{
    Initialize(bufferData.data());
}



void BufferObject::BindAsUniformBuffer(GLuint bindingPoint) const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, mHandle);
    DEV_EXCEPTION(QueryUniformBufferBinding(bindingPoint) != mHandle,
                  "Binding point mismatch - API did not return the set value");
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



bool BufferObject::IsBoundToUniformBufferBindingPoint(GLuint bindingPoint) const
{
    return mHandle == QueryUniformBufferBinding(bindingPoint);
}



GLuint BufferObject::QueryUniformBufferBinding(GLuint bindingPoint)
{
    GLint boundBufferHandle = -1;
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, bindingPoint, &boundBufferHandle);
    return static_cast<GLuint>(boundBufferHandle);
}



void BufferObject::SetData(const Vector<U8>& data, GLint offset)
{
    SetData(data.data(), static_cast<GLsizei>(data.size()), offset);
}



void BufferObject::SetData(const Vector<F32>& data, GLint offset)
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
