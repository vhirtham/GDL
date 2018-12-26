#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/rendering/openGL/core/bufferObject.h"

#include "test/tools/ExceptionChecks.h"
#include "test/tools/GetRenderWindowGLUT.h"

using namespace GDL;
using namespace GDL::OpenGL;



template <typename _BufferObjectType>
void CheckBufferObjectData(const _BufferObjectType& bufferObject, const GLsizei expectedSize, GLenum expectedUsage)
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



template <typename _BufferObjectType>
void TestConstructionWithoutData()
{
    constexpr GLsizei size = 120;
    constexpr GLenum usage = GL_STATIC_DRAW;

    _BufferObjectType bufferObject(size, usage);

    CheckBufferObjectData(bufferObject, size, usage);
}



BOOST_AUTO_TEST_CASE(Construction_without_data)
{
    GetRenderWindowGLUT();
    ContextManagerGLUT::Instance().GetDebugMessageHandler().SetSeverityLevel(GL_DEBUG_SEVERITY_LOW);

    TestConstructionWithoutData<BufferObject>();
}


template <typename _BufferObjectType>
void TestConstructionWithData()
{
    constexpr U32 numData = 180;
    constexpr GLsizei size = numData * sizeof(F32);
    constexpr GLenum usage = GL_DYNAMIC_READ;

    Vector<F32> bufferData(numData, 42.f);

    _BufferObjectType bufferObject(bufferData, usage);

    CheckBufferObjectData(bufferObject, size, usage);
}


BOOST_AUTO_TEST_CASE(Construction_with_data)
{
    TestConstructionWithData<BufferObject>();
}


template <typename _BufferObjectType>
void TestSetData()
{
    constexpr U32 numData = 10;
    constexpr GLsizei size = 120;
    constexpr GLint maxPossibleOffset = size - numData * sizeof(F32);
    constexpr GLenum usage = GL_STATIC_DRAW;


    _BufferObjectType bufferObject(size, usage);

    Vector<F32> values(numData, 13.f);

    bufferObject.SetData(values);
    bufferObject.SetData(values, maxPossibleOffset);
    GDL_CHECK_THROW_DEV_DISABLE(bufferObject.SetData(values, maxPossibleOffset + 1), Exception);
}



BOOST_AUTO_TEST_CASE(Set_Data)
{
    TestSetData<BufferObject>();
}



BOOST_AUTO_TEST_CASE(UBO_binding_point)
{
    constexpr GLsizei size = 120;
    constexpr GLenum usage = GL_STATIC_DRAW;
    constexpr GLuint bindingPoint = 2;

    BufferObject ubo(size, usage);


    GLint queriedBufferHandle = -1;
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, bindingPoint, &queriedBufferHandle);
    BOOST_CHECK(!ubo.IsBoundToUniformBufferBindingPoint(bindingPoint));

    ubo.BindAsUniformBuffer(bindingPoint);

    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, bindingPoint, &queriedBufferHandle);
    BOOST_CHECK(ubo.IsBoundToUniformBufferBindingPoint(bindingPoint));
}
