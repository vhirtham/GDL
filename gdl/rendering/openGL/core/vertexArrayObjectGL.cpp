#include "gdl/rendering/openGL/core/vertexArrayObjectGL.h"

#include "gdl/rendering/openGL/core/bufferObjectGL.h"

#include <cassert>

namespace GDL
{
namespace OpenGL
{

VertexArrayObject::~VertexArrayObject()
{
    for (GLint i = mNumAttributes; i > 0; --i)
        glDisableVertexArrayAttrib(mHandle, i - 1);
    glDeleteVertexArrays(1, &mHandle);
}

void VertexArrayObject::AddAttribute(const BufferObject& bufferObject, GLint numVertexValues, GLenum type,
                                     GLsizei stride, GLint offset, GLuint relOffset, GLboolean normalized)
{
    assert(stride > 0);

    // https://stackoverflow.com/questions/32739297/direct-state-access-with-vertex-buffers
    // Make attribute selectable
    const GLuint attribIndex = mNumAttributes;
    // Add assertion that checks if the attribute is already active (and therefore used) - Check link below
    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glGetVertexAttrib.xml


    const GLuint bindingPoint = mNumAttributes;
    glEnableVertexArrayAttrib(mHandle, attribIndex);
    glVertexArrayVertexBuffer(mHandle, bindingPoint, bufferObject.GetHandle(), offset, stride);
    glVertexArrayAttribFormat(mHandle, attribIndex, numVertexValues, type, normalized, relOffset);
    glVertexArrayAttribBinding(mHandle, attribIndex, bindingPoint);
    assert(glGetError() == GL_NO_ERROR);
    ++mNumAttributes;
}

VertexArrayObject::VertexArrayObject()
    : mHandle{0}
    , mNumAttributes{0}
{
    Initialize();
}

void VertexArrayObject::Initialize()
{
    glCreateVertexArrays(1, &mHandle);
    assert(mHandle > 0);
    assert(glGetError() == GL_NO_ERROR);
}

} // namespace OpenGL
} // namespace GDL
