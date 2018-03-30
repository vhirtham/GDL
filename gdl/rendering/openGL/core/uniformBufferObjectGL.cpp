#include "gdl/rendering/openGL/core/uniformBufferObjectGL.h"


#include <cassert>

GDL::UniformBufferObject::UniformBufferObject(GLuint size, GLenum usage)
    : mSize(size)
    , mUsage(usage)
{
    const std::vector<U8> buffer(size, 0);
    Initialize(buffer, usage);
}

GDL::UniformBufferObject::UniformBufferObject(const std::vector<GDL::U8>& buffer, GLenum usage)
    : mSize(buffer.size())
    , mUsage(usage)
{
    Initialize(buffer, usage);
}

GDL::UniformBufferObject::~UniformBufferObject()
{
    glDeleteBuffers(1, &mHandle);
}

void GDL::UniformBufferObject::Initialize(const std::vector<GDL::U8>& buffer, GLenum usage)
{
    glCreateBuffers(1, &mHandle);
    glNamedBufferData(mHandle, buffer.size(), buffer.data(), usage);
    assert(glGetError() == GL_NO_ERROR);
}
