#include "gdl/rendering/openGL/core/vertexArrayObjectGL.h"

#include "gdl/rendering/openGL/core/bufferObjectGL.h"

#include <cassert>

namespace GDL
{
namespace OpenGL
{



VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &mHandle);
    assert(glGetError() == GL_NO_ERROR);
}



void VertexArrayObject::EnableAttribute(GLuint attributeIndex, GLint numVertexValues, GLenum type, GLuint relOffset,
                                        GLboolean normalized)
{
    assert(CheckAttributeEnabled(attributeIndex) == false && "Attribute already enabled!");

    const GLuint bindingPoint = attributeIndex;

    glEnableVertexArrayAttrib(mHandle, attributeIndex);
    glVertexArrayAttribFormat(mHandle, attributeIndex, numVertexValues, type, normalized, relOffset);
    glVertexArrayAttribBinding(mHandle, attributeIndex, bindingPoint);

    assert(glGetError() == GL_NO_ERROR);
}



void VertexArrayObject::EnableAttribute(GLuint attributeIndex, const BufferObject& bufferObject, GLint numVertexValues,
                                        GLenum type, GLsizei stride, GLint offset, GLuint relOffset,
                                        GLboolean normalized)
{
    EnableAttribute(attributeIndex, numVertexValues, type, relOffset, normalized);
    SetAttributeBuffer(attributeIndex, bufferObject, stride, offset);
}



void VertexArrayObject::Bind() const
{
    assert(CheckVertexAttribsBoundToBuffer() && "One or more attributes are not bound to a buffer!");
    glBindVertexArray(mHandle);
}



void VertexArrayObject::SetAttributeBuffer(GLuint attributeIndex, const BufferObject& bufferObject, GLsizei stride,
                                           GLint offset) const
{
    assert(stride > 0);
    assert(offset > -1);

    const GLuint bindingPoint = attributeIndex;
    glVertexArrayVertexBuffer(mHandle, bindingPoint, bufferObject.GetHandle(), offset, stride);

    assert(QueryAttributeBuffer(attributeIndex) == bufferObject.GetHandle());
    assert(glGetError() == GL_NO_ERROR);
}



bool VertexArrayObject::CheckAttributeEnabled(GLuint attributeIndex) const
{
    GLint enabled = -1;
    glGetVertexArrayIndexediv(mHandle, attributeIndex, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    return enabled;
}



VertexArrayObject::VertexArrayObject()
    : mHandle{0}
{
    Initialize();
}



void VertexArrayObject::Initialize()
{
    glCreateVertexArrays(1, &mHandle);
    assert(mHandle > 0);
    assert(glGetError() == GL_NO_ERROR);
}



bool VertexArrayObject::CheckVertexAttribsBoundToBuffer() const
{
    GLint maxAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
    for (GLint i = 0; i < maxAttribs; ++i)
    {
        if (CheckAttributeEnabled(i))
            if (QueryAttributeBuffer(i) == 0)
                return false;
    }
    return true;
}



GLuint VertexArrayObject::QueryAttributeBuffer(GLuint attributeIndex) const
{
    GLint buffer = -1;
    glGetVertexArrayIndexediv(mHandle, attributeIndex, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &buffer);
    assert(glGetError() == GL_NO_ERROR);
    return buffer;
}

} // namespace OpenGL
} // namespace GDL
