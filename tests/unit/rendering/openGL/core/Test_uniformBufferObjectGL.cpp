#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/GDLTypedefs.h"
#include "gdl/rendering/openGL/core/renderWindowGL.h"
#include "gdl/rendering/openGL/core/uniformBufferObjectGL.h"

using namespace GDL;
using namespace GDL::OpenGL;

RenderWindow& GetRenderWindow()
{
    static bool initialized = false;
    static RenderWindow renderWindow;
    if (!initialized)
    {
        renderWindow.Initialize();
        initialized = true;
    }
    return renderWindow;
}

BOOST_AUTO_TEST_CASE(Construction_And_Binding)
{
    GetRenderWindow();

    // Ctor with size test
    constexpr GLsizei size_A = 120;
    constexpr GLuint binding_A = 2;
    constexpr GLenum usage_A = GL_STATIC_DRAW;

    UniformBufferObject uniformBufferObject_A(size_A, usage_A);
    uniformBufferObject_A.SetBindingPoint(binding_A);

    // Check data in class
    BOOST_CHECK(uniformBufferObject_A.GetBindingPoint() == binding_A);
    BOOST_CHECK(uniformBufferObject_A.GetSize() == size_A);
    BOOST_CHECK(uniformBufferObject_A.GetUsage() == usage_A);

    // Check data on OpenGL side
    GLint queriedBufferSize_A = -1;
    GLint queriedBufferHandle_A = -1;
    GLint queriedBufferUsage_A = GL_NONE;
    glGetNamedBufferParameteriv(uniformBufferObject_A.GetHandle(), GL_BUFFER_SIZE, &queriedBufferSize_A);
    glGetNamedBufferParameteriv(uniformBufferObject_A.GetHandle(), GL_BUFFER_USAGE, &queriedBufferUsage_A);
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, binding_A, &queriedBufferHandle_A);
    BOOST_CHECK(uniformBufferObject_A.GetHandle() == static_cast<GLuint>(queriedBufferHandle_A));
    BOOST_CHECK(uniformBufferObject_A.GetSize() == static_cast<GLsizei>(queriedBufferSize_A));
    BOOST_CHECK(uniformBufferObject_A.GetUsage() == static_cast<GLenum>(queriedBufferUsage_A));


    // Ctor with buffer test
    constexpr GLsizei size_B = 180;
    constexpr GLuint binding_B = 7;
    constexpr GLenum usage_B = GL_DYNAMIC_READ;

    std::vector<U8> buffer_B(size_B, 42);

    UniformBufferObject uniformBufferObject_B(buffer_B, usage_B);
    uniformBufferObject_B.SetBindingPoint(binding_B);

    // Check data in class
    BOOST_CHECK(uniformBufferObject_B.GetBindingPoint() == binding_B);
    BOOST_CHECK(uniformBufferObject_B.GetSize() == size_B);
    BOOST_CHECK(uniformBufferObject_B.GetUsage() == usage_B);

    // Check data on OpenGL side
    GLint queriedBufferSize_B = -1;
    GLint queriedBufferHandle_B = -1;
    GLint queriedBufferUsage_B = GL_NONE;
    glGetNamedBufferParameteriv(uniformBufferObject_B.GetHandle(), GL_BUFFER_SIZE, &queriedBufferSize_B);
    glGetNamedBufferParameteriv(uniformBufferObject_B.GetHandle(), GL_BUFFER_USAGE, &queriedBufferUsage_B);
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, binding_B, &queriedBufferHandle_B);
    BOOST_CHECK(uniformBufferObject_B.GetHandle() == static_cast<GLuint>(queriedBufferHandle_B));
    BOOST_CHECK(uniformBufferObject_B.GetSize() == static_cast<GLsizei>(queriedBufferSize_B));
    BOOST_CHECK(uniformBufferObject_B.GetUsage() == static_cast<GLenum>(queriedBufferUsage_B));
}

BOOST_AUTO_TEST_CASE(Set_Data)
{
    GetRenderWindow();

    constexpr GLsizei size = 120;
    constexpr GLenum usage = GL_STATIC_DRAW;
    UniformBufferObject uniformBufferObject(size, usage);

    std::vector<F32> valA(30, 13.f);
    uniformBufferObject.SetData(valA);

    std::vector<U8> valB(60, 111);
    uniformBufferObject.SetData(valB);
    uniformBufferObject.SetData(valB, 60);
}
