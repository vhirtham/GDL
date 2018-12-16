#pragma once


#include "gdl/base/fundamentalTypes.h"

#include <GL/glew.h>

namespace GDL::OpenGL
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


    //! @brief Binds the vertex array object
    void Bind() const;

    //! @brief Enables an attribute without binding a buffer object to the attributes binding Point. In this function
    //! overload the binding point equals the attribute index.
    //! @param attributeIndex: The index of the attribute that should be enabled
    //! @param numVertexValues: The attributes number of values per vertex
    //! @param type: Value type enum
    //! @param relOffset: The distance between elements within the buffer
    //! @param normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted
    //! directly as fixed-point values (GL_FALSE) when they are accessed. This parameter is ignored if type is GL_FIXED.
    void EnableAttribute(GLuint attributeIndex, GLint numVertexValues, GLenum type, GLuint relOffset = 0,
                         GLboolean normalized = GL_FALSE);

    //! @brief Enables an attribute without binding a buffer object to the attributes binding Point.
    //! @param attributeIndex: The index of the attribute that should be enabled
    //! @param bindingPoint: Binding point to which the enabled attribute should be bound to.
    //! @param numVertexValues: The attributes number of values per vertex
    //! @param type: Value type enum
    //! @param relOffset: The distance between elements within the buffer
    //! @param normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted
    //! directly as fixed-point values (GL_FALSE) when they are accessed. This parameter is ignored if type is GL_FIXED.
    void EnableAttribute(GLuint attributeIndex, GLuint bindingPoint, GLint numVertexValues, GLenum type,
                         GLuint relOffset, GLboolean normalized);

    //! @brief Enables an attribute and connects it with the provided buffer object. In this function
    //! overload the used binding point equals the attributes index.
    //! @param attributeIndex: The index of the attribute that should be enabled
    //! @param bufferObject: Buffer object that provides the data for the attribute
    //! @param numVertexValues: The attributes number of values per vertex
    //! @param type: Value type enum
    //! @param stride: Stride in bytes between two vertices in the buffer
    //! @param offset: Offset of the attributes first vertex data
    //! @param relOffset: The distance between elements within the buffer
    //! @param normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted
    //! directly as fixed-point values (GL_FALSE) when they are accessed. This parameter is ignored if type is GL_FIXED.
    void EnableAttribute(GLuint attributeIndex, const BufferObject& bufferObject, GLint numVertexValues, GLenum type,
                         GLsizei stride, GLint offset = 0, GLuint relOffset = 0, GLboolean normalized = GL_FALSE);

    //! @brief Enables an attribute and connects it with the provided buffer object via the specified binding point
    //! @param attributeIndex: The index of the attribute that should be enabled
    //! @param bindingPoint: Binding point to which the enabled attribute and the buffer should be bound.
    //! @param bufferObject: Buffer object that provides the data for the attribute
    //! @param numVertexValues: The attributes number of values per vertex
    //! @param type: Value type enum
    //! @param stride: Stride in bytes between two vertices in the buffer
    //! @param offset: Offset of the attributes first vertex data
    //! @param relOffset: The distance between elements within the buffer
    //! @param normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted
    //! directly as fixed-point values (GL_FALSE) when they are accessed. This parameter is ignored if type is GL_FIXED.
    void EnableAttribute(GLuint attributeIndex, GLuint bindingPoint, const BufferObject& bufferObject,
                         GLint numVertexValues, GLenum type, GLsizei stride, GLint offset = 0, GLuint relOffset = 0,
                         GLboolean normalized = GL_FALSE);


    //! @brief Checks if all active attributes are bound to a buffer
    //! @return True / False
    //! @remark This function is potentially slow. You should only use it for debugging purposes
    bool QueryAreAllVertexAttribsBoundToBuffers() const;

    //! @brief Queries the handle of the buffer currenlty bound to the attribute
    //! @param attributeIndex: The index of the attribute
    //! @return Handle of the currently bound buffer
    //! @remark This function is potentially slow. You should only use it for debugging purposes
    GLuint QueryAttributeBuffer(GLuint attributeIndex) const;

    //! @brief Checks if a vertex attribute is enabled
    //! @brief atributeIndex: The index of the attribute
    //! @return True / False
    //! @remark This function is potentially slow. You should only use it for debugging purposes
    bool QueryIsAttributeEnabled(GLuint attributeIndex) const;

    //! @brief Queries the number of enabled attributes
    //! @return Number of enabled attributes
    U32 QueryNumEnabledAttributes() const;

    //! @brief Binds a buffer object to an attribute. This function overload assumes that the binding point of the
    //! attribute equals its index.
    //! @param attributeIndex: The index of the attribute that should be connected with the buffer
    //! @param bufferObject: Buffer object that provides the data for the attribute
    //! @param stride: Stride in bytes between two vertices in the buffer
    //! @param offset: Offset of the attributes first vertex data
    void SetAttributeBuffer(GLuint attributeIndex, const BufferObject& bufferObject, GLsizei stride,
                            GLint offset = 0);

    //! @brief Binds a buffer object to an attribute
    //! @param attributeIndex: The index of the attribute that should be connected with the buffer
    //! @param bindingPoint: Binding point to which the buffer will be bound.
    //! @param bufferObject: Buffer object that provides the data for the attribute
    //! @param stride: Stride in bytes between two vertices in the buffer
    //! @param offset: Offset of the attributes first vertex data
    //! @remark The attribute index is only used for debugging purposes inside of this function. If the DEV_EXCEPTION
    //! macro is disabled, it has no effect
    void SetAttributeBuffer(GLuint attributeIndex, GLuint bindingPoint, const BufferObject& bufferObject,
                            GLsizei stride, GLint offset = 0);


private:
    //! @brief Initializes the vertex array object
    void Initialize();
};

} // namespace GDL::OpenGL
