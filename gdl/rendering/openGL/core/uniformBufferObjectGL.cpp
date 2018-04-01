#include "gdl/rendering/openGL/core/uniformBufferObjectGL.h"


#include <cassert>
#include <vector>

GDL::OpenGL::UniformBufferObject::UniformBufferObject(GLuint size, GLenum usage)
    : mBufferObject(size, usage)
{
}



void GDL::OpenGL::UniformBufferObject::SetBindingPoint(GLint bindingPoint)
{
    mBindingPoint = bindingPoint;
    glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mBufferObject.GetHandle());
    assert(BindingPointValid());
}


bool GDL::OpenGL::UniformBufferObject::BindingPointValid() const
{
    GLint boundBufferHandle = -1;
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, mBindingPoint, &boundBufferHandle);
    return boundBufferHandle == static_cast<GLint>(mBufferObject.GetHandle());
}
