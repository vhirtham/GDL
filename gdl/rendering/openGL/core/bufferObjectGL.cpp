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

BufferObject::BufferObject(const std::vector<U8>& buffer, GLenum usage)
    : mSize(buffer.size())
    , mUsage(usage)
{
    Initialize(buffer, usage);
}

BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &mHandle);
}

void BufferObject::Initialize(const std::vector<U8>& buffer, GLenum usage)
{
    glCreateBuffers(1, &mHandle);
    glNamedBufferData(mHandle, buffer.size(), buffer.data(), usage);
    assert(glGetError() == GL_NO_ERROR);
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

} // namespace OpenGl
} // namespace GDL
