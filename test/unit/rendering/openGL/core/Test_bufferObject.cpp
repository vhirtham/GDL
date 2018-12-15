#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/rendering/openGL/core/bufferObject.h"

#include "test/tools/ExceptionChecks.h"
#include "test/tools/GetRenderWindowGLUT.h"

using namespace GDL;
using namespace GDL::OpenGL;


void CheckBufferObjectData(const BufferObject& bufferObject, const GLsizei expectedSize, GLenum expectedUsage)
{
    BOOST_CHECK(bufferObject.GetSize() == expectedSize);
    BOOST_CHECK(bufferObject.GetUsage() == expectedUsage);

    GLint queriedBufferSize = -1;
    GLint queriedBufferUsage = GL_NONE;
    glGetNamedBufferParameteriv(bufferObject.GetHandle(), GL_BUFFER_SIZE, &queriedBufferSize);
    glGetNamedBufferParameteriv(bufferObject.GetHandle(), GL_BUFFER_USAGE, &queriedBufferUsage);
    BOOST_CHECK(bufferObject.GetSize() == static_cast<GLsizei>(queriedBufferSize));
    BOOST_CHECK(bufferObject.GetUsage() == static_cast<GLenum>(queriedBufferUsage));
}



BOOST_AUTO_TEST_CASE(Construction_without_data)
{
    GetRenderWindowGLUT();
    ContextGLUT::Instance().GetDebugMessageHandler().SetSeverityLevel(GL_DEBUG_SEVERITY_LOW);

    constexpr GLsizei size = 120;
    constexpr GLenum usage = GL_STATIC_DRAW;

    BufferObject bufferObject(size, usage);

    CheckBufferObjectData(bufferObject, size, usage);
}



BOOST_AUTO_TEST_CASE(Construction_with_data)
{
    // Ctor with buffer test
    constexpr U32 numData = 180;
    constexpr GLsizei size = numData * sizeof(F32);
    constexpr GLenum usage = GL_DYNAMIC_READ;

    Vector<F32> bufferData(numData, 42.f);

    BufferObject bufferObject(bufferData, usage);

    CheckBufferObjectData(bufferObject, size, usage);
}



BOOST_AUTO_TEST_CASE(Set_Data)
{
    constexpr U32 numData = 10;
    constexpr GLsizei size = 120;
    constexpr GLint maxPossibleOffset = size - numData * sizeof(F32);
    constexpr GLenum usage = GL_STATIC_DRAW;


    BufferObject bufferObject(size, usage);

    Vector<F32> values(numData, 13.f);

    bufferObject.SetData(values);
    bufferObject.SetData(values, maxPossibleOffset);
    GDL_CHECK_THROW_DEV_DISABLE(bufferObject.SetData(values, maxPossibleOffset + 1), Exception);
}
