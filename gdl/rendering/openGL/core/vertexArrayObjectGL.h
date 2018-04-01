#pragma once


#include <GL/glew.h>

namespace GDL
{
namespace OpenGL
{

class BufferObject;

class VertexArrayObject
{
    GLuint mHandle = 0;
    GLint mNumAttributes = 0;

public:
    VertexArrayObject();
    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject(VertexArrayObject&&) = delete;
    VertexArrayObject& operator=(const VertexArrayObject&) = delete;
    VertexArrayObject& operator=(VertexArrayObject&&) = delete;
    ~VertexArrayObject();

    void Bind() const
    {
        glBindVertexArray(mHandle);
    }

    //! @brief Activates another attribute using the provided buffer object
    //! @param bufferObject: Buffer object that provides the data for the attribute
    //! @param numVertexValues: The attributes number of values per vertex
    //! @param type: Value type enum
    //! @param stride: Stride in bytes between two vertices
    //! @param offset: Offset of the attributes first vertex data
    //! @param relOffset: --- see OpenGL documentation
    //! @param normalized: --- see OpenGL documentation
    void AddAttribute(const BufferObject& bufferObject, GLint numVertexValues, GLenum type, GLsizei stride,
                      GLint offset = 0, GLuint relOffset = 0, GLboolean normalized = GL_FALSE);

private:
    //! @brief Initializes the vertex array object
    void Initialize();
};
}
}
