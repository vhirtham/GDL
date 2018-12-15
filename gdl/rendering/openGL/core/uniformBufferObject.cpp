#include "gdl/rendering/openGL/core/uniformBufferObject.h"

#include "gdl/base/exception.h"


namespace GDL::OpenGL
{

UniformBufferObject::UniformBufferObject(GLsizei size, GLenum usage)
    : mBufferObject(size, usage)
{
}



UniformBufferObject::UniformBufferObject(const Vector<GDL::F32>& bufferData, GLenum usage)
    : mBufferObject(bufferData, usage)
{
}



GLint UniformBufferObject::GetBindingPoint() const
{
    return mBindingPoint;
}



GLuint UniformBufferObject::GetHandle() const
{
    return mBufferObject.GetHandle();
}



GLsizei UniformBufferObject::GetSize() const
{
    return mBufferObject.GetSize();
}



GLenum UniformBufferObject::GetUsage() const
{
    return mBufferObject.GetUsage();
}



void UniformBufferObject::SetBindingPoint(GLint bindingPoint)
{
    mBindingPoint = bindingPoint;
    glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mBufferObject.GetHandle());
    DEV_EXCEPTION(!IsBindingPointValid(), "Binding point mismatch - API did not return the set value");
}



void UniformBufferObject::SetData(Vector<GDL::F32> data, GLint offset)
{
    mBufferObject.SetData(data, offset);
}



bool UniformBufferObject::IsBindingPointValid() const
{
    GLint boundBufferHandle = -1;
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, mBindingPoint, &boundBufferHandle);
    return boundBufferHandle == static_cast<GLint>(mBufferObject.GetHandle());
}

} // namespace GDL::OpenGL
