#include <boost/test/unit_test.hpp>

#include "gdl/base/Exception.h"
#include "gdl/rendering/openGL/core/renderWindowGL.h"
#include "gdl/rendering/openGL/core/bufferObjectGL.h"

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

BOOST_AUTO_TEST_CASE(Construction)
{
    GetRenderWindow();

    // Ctor with size test
    constexpr GLsizei size_A = 120;
    constexpr GLenum usage_A = GL_STATIC_DRAW;

    BufferObject bufferObject_A(size_A, usage_A);

    // Check data in class
    BOOST_CHECK(bufferObject_A.GetSize() == size_A);
    BOOST_CHECK(bufferObject_A.GetUsage() == usage_A);

    // Check data on OpenGL side
    GLint queriedBufferSize_A = -1;
    GLint queriedBufferUsage_A = GL_NONE;
    glGetNamedBufferParameteriv(bufferObject_A.GetHandle(), GL_BUFFER_SIZE, &queriedBufferSize_A);
    glGetNamedBufferParameteriv(bufferObject_A.GetHandle(), GL_BUFFER_USAGE, &queriedBufferUsage_A);
    BOOST_CHECK(bufferObject_A.GetSize() == static_cast<GLsizei>(queriedBufferSize_A));
    BOOST_CHECK(bufferObject_A.GetUsage() == static_cast<GLenum>(queriedBufferUsage_A));


    // Ctor with buffer test
    constexpr GLsizei size_B = 180;
    constexpr GLenum usage_B = GL_DYNAMIC_READ;

    std::vector<U8> buffer_B(size_B, 42);

    BufferObject bufferObject_B(buffer_B, usage_B);

    // Check data in class
    BOOST_CHECK(bufferObject_B.GetSize() == size_B);
    BOOST_CHECK(bufferObject_B.GetUsage() == usage_B);

    // Check data on OpenGL side
    GLint queriedBufferSize_B = -1;
    GLint queriedBufferUsage_B = GL_NONE;
    glGetNamedBufferParameteriv(bufferObject_B.GetHandle(), GL_BUFFER_SIZE, &queriedBufferSize_B);
    glGetNamedBufferParameteriv(bufferObject_B.GetHandle(), GL_BUFFER_USAGE, &queriedBufferUsage_B);
    BOOST_CHECK(bufferObject_B.GetSize() == static_cast<GLsizei>(queriedBufferSize_B));
    BOOST_CHECK(bufferObject_B.GetUsage() == static_cast<GLenum>(queriedBufferUsage_B));
}

BOOST_AUTO_TEST_CASE(Set_Data)
{
    GetRenderWindow();

    constexpr GLsizei size = 120;
    constexpr GLenum usage = GL_STATIC_DRAW;
    BufferObject bufferObject(size, usage);

    std::vector<F32> valA(30, 13.f);
    bufferObject.SetData(valA);

    std::vector<U8> valB(60, 111);
    bufferObject.SetData(valB);
    bufferObject.SetData(valB, 60);
}
