#pragma once


#include <GL/glew.h>

namespace GDL
{
namespace OpenGL
{

class BufferObject;

//! @brief Wrapper class for vertex array objects
//! @remark: Uses direct state access as described here:
//! https://stackoverflow.com/questions/32739297/direct-state-access-with-vertex-buffers
class VertexArrayObject
{
    GLuint mHandle = 0;

public:
    VertexArrayObject();
    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject(VertexArrayObject&&) = delete;
    VertexArrayObject& operator=(const VertexArrayObject&) = delete;
    VertexArrayObject& operator=(VertexArrayObject&&) = delete;
    ~VertexArrayObject();


    //! @brief Enables an attribute without binding a buffer object
    //! @param attributeIndex: The index of the attribute that should be enabled
    //! @param numVertexValues: The attributes number of values per vertex
    //! @param type: Value type enum
    //! @param relOffset: --- see OpenGL documentation
    //! @param normalized: --- see OpenGL documentation
    void EnableAttribute(GLuint attributeIndex, GLint numVertexValues, GLenum type, GLuint relOffset = 0,
                         GLboolean normalized = GL_FALSE);


    //! @brief Enables an attribute using the provided buffer object
    //! @param attributeIndex: The index of the attribute that should be enabled
    //! @param bufferObject: Buffer object that provides the data for the attribute
    //! @param numVertexValues: The attributes number of values per vertex
    //! @param type: Value type enum
    //! @param stride: Stride in bytes between two vertices in the buffer
    //! @param offset: Offset of the attributes first vertex data
    //! @param relOffset: --- see OpenGL documentation
    //! @param normalized: --- see OpenGL documentation
    void EnableAttribute(GLuint attributeIndex, const BufferObject& bufferObject, GLint numVertexValues, GLenum type,
                         GLsizei stride, GLint offset = 0, GLuint relOffset = 0, GLboolean normalized = GL_FALSE);

    //! @brief Binds the vertex array object
    void Bind() const;


    //! @brief Binds a buffer object to an attribute
    //! @param attributeIndex: The index of the attribute that should be enabled
    //! @param bufferObject: Buffer object that provides the data for the attribute
    //! @param stride: Stride in bytes between two vertices in the buffer
    //! @param offset: Offset of the attributes first vertex data
    void SetAttributeBuffer(GLuint attributeIndex, const BufferObject& bufferObject, GLsizei stride,
                            GLint offset = 0) const;

    //! @brief Checks if a vertex attribute is enabled
    //! @brief atributeIndex: The index of the attribute
    //! @return true/false
    //! @remark This function is potentially slow. You should only use it for debugging purposes
    bool CheckAttributeEnabled(GLuint attributeIndex) const;


    //! @brief Checks if all active attributes are bound to a buffer
    //! @return true/false
    //! @remark This function is potentially slow. You should only use it for debugging purposes
    bool CheckVertexAttribsBoundToBuffer() const;


    //! @brief Queries the handle of the buffer currenlty bound to the attribute
    //! @param attributeIndex: The index of the attribute
    //! @return Handle of the currently bound buffer
    //! @remark This function is potentially slow. You should only use it for debugging purposes
    GLuint QueryAttributeBuffer(GLuint attributeIndex) const;

private:
    //! @brief Initializes the vertex array object
    void Initialize();
};
}
}
