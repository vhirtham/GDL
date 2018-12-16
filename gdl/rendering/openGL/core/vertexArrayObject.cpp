#include "gdl/rendering/openGL/core/vertexArrayObject.h"

#include "gdl/rendering/openGL/core/bufferObject.h"



namespace GDL::OpenGL
{

VertexArrayObject::VertexArrayObject()
    : mHandle{0}
{
    Initialize();
}



VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &mHandle);
}



void VertexArrayObject::Bind() const
{
    DEBUG_EXCEPTION(!QueryAreAllVertexAttribsBoundToBuffers(), "One or more attributes are not bound to a buffer!");
    glBindVertexArray(mHandle);
}



void VertexArrayObject::EnableAttribute(GLuint attributeIndex, GLuint bindingPoint, GLenum type, GLint numVertexValues,
                                        GLuint relOffset, GLboolean normalized)
{
    DEV_EXCEPTION(QueryIsAttributeEnabled(attributeIndex), "Attribute already enabled!");

    glEnableVertexArrayAttrib(mHandle, attributeIndex);
    glVertexArrayAttribFormat(mHandle, attributeIndex, numVertexValues, type, normalized, relOffset);
    glVertexArrayAttribBinding(mHandle, attributeIndex, bindingPoint);
}



void VertexArrayObject::EnableAttribute(GLuint attributeIndex, GLuint bindingPoint, const BufferObject& bufferObject,
                                        GLenum type, GLint numVertexValues, GLsizei stride, GLint offset,
                                        GLuint relOffset, GLboolean normalized)
{
    EnableAttribute(attributeIndex, bindingPoint, type, numVertexValues, relOffset, normalized);
    SetBufferBinding(bindingPoint, bufferObject, stride, offset);
}



bool VertexArrayObject::QueryAreAllVertexAttribsBoundToBuffers() const
{
    GLint maxAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);

    for (GLuint i = 0; i < static_cast<GLuint>(maxAttribs); ++i)
        if (QueryIsAttributeEnabled(i) && QueryAttributeBuffer(i) == 0)
            return false;

    return true;
}



GLuint VertexArrayObject::QueryAttributeBuffer(GLuint attributeIndex) const
{
    //    GLint bufferHandle = -1;
    //    glGetVertexArrayIndexediv(mHandle, attributeIndex, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bufferHandle);
    //    return buffer;

    GLint boundVAO = -1;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);

    glBindVertexArray(mHandle);
    GLint bufferHandle = -1;
    glGetVertexAttribiv(attributeIndex, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bufferHandle);

    glBindVertexArray(boundVAO);

    return static_cast<GLuint>(bufferHandle);
}



bool VertexArrayObject::QueryIsAttributeEnabled(GLuint attributeIndex) const
{
    GLint enabled = -1;
    glGetVertexArrayIndexediv(mHandle, attributeIndex, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    return enabled;
}



U32 VertexArrayObject::QueryNumEnabledAttributes() const
{
    U32 numEnabledAttribs = 0;

    GLint maxAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);

    for (GLuint i = 0; i < static_cast<GLuint>(maxAttribs); ++i)
        if (QueryIsAttributeEnabled(i))
            ++numEnabledAttribs;

    return numEnabledAttribs;
}



void VertexArrayObject::SetBufferBinding(GLuint bindingPoint, const BufferObject& bufferObject, GLsizei stride,
                                         GLint offset)
{
    DEV_EXCEPTION(stride <= 0, "Stride must be a >= 0.");
    DEV_EXCEPTION(offset < 0, "Offset can't be negative.");

    glVertexArrayVertexBuffer(mHandle, bindingPoint, bufferObject.GetHandle(), offset, stride);
}



void VertexArrayObject::Initialize()
{
    glCreateVertexArrays(1, &mHandle);
}


} // namespace GDL::OpenGL
