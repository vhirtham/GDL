#include "gdl/rendering/openGL/core/uniformBufferObjectGL.h"


#include <cassert>
#include <vector>

GDL::OpenGL::UniformBufferObject::UniformBufferObject(GLuint size, GLenum usage)
    : mSize(size)
    , mUsage(usage)
{
    const std::vector<U8> buffer(size, 0);
    Initialize(buffer, usage);
}

GDL::OpenGL::UniformBufferObject::UniformBufferObject(const std::vector<GDL::U8>& buffer, GLenum usage)
    : mSize(buffer.size())
    , mUsage(usage)
{
    Initialize(buffer, usage);
}

GDL::OpenGL::UniformBufferObject::~UniformBufferObject()
{
    glDeleteBuffers(1, &mHandle);
}

void GDL::OpenGL::UniformBufferObject::SetBindingPoint(GLint bindingPoint)
{
    mBindingPoint = bindingPoint;
    glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mHandle);
    assert(BindingPointValid());
}

void GDL::OpenGL::UniformBufferObject::Initialize(const std::vector<GDL::U8>& buffer, GLenum usage)
{
    glCreateBuffers(1, &mHandle);
    glNamedBufferData(mHandle, buffer.size(), buffer.data(), usage);
    assert(mHandle > 0);
    assert(glGetError() == GL_NO_ERROR);
}

bool GDL::OpenGL::UniformBufferObject::BindingPointValid() const
{
    GLint boundBufferHandle = -1;
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, mBindingPoint, &boundBufferHandle);
    return boundBufferHandle == static_cast<GLint>(mHandle);
}

template <>
void GDL::OpenGL::UniformBufferObject::SetData<std::vector<GDL::U8>>(std::vector<U8> data, GLint offset) const
{
    assert(static_cast<GLsizei>(offset + data.size() * sizeof(U8)) <= mSize);
    glNamedBufferSubData(mHandle, offset, data.size() * sizeof(U8), data.data());
}

template <>
void GDL::OpenGL::UniformBufferObject::SetData<std::vector<GDL::F32>>(std::vector<F32> data, GLint offset) const
{
    assert(static_cast<GLsizei>(offset + data.size() * sizeof(F32)) <= mSize);
    glNamedBufferSubData(mHandle, offset, data.size() * sizeof(F32), data.data());
}
