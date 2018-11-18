#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/rendering/openGL/core/bufferObjectGL.h"
#include "gdl/rendering/openGL/core/renderWindowGL.h"
#include "gdl/rendering/openGL/core/vertexArrayObjectGL.h"

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
    VertexArrayObject vao;
}


BOOST_AUTO_TEST_CASE(Add_Attributes)
{
    GetRenderWindow();
    VertexArrayObject vao;
    BufferObject vbo1(120, GL_STATIC_DRAW);
    BufferObject vbo2(40, GL_STATIC_DRAW);
    vao.EnableAttribute(0, vbo1, 1, GL_FLOAT, 2, 0);
    vao.EnableAttribute(1, 1, GL_FLOAT);
    vao.SetAttributeBuffer(1, vbo1, 2, 1);
    vao.EnableAttribute(2, vbo1, 1, GL_FLOAT, 1, 80);
    vao.EnableAttribute(3, vbo2, 1, GL_FLOAT, 1);

    // Add a program and call glDrawArrays to see if everthing works
}

BOOST_AUTO_TEST_CASE(State_Check_Functions)
{
    GetRenderWindow();

    VertexArrayObject vao;
    BufferObject vbo1(120, GL_STATIC_DRAW);
    BufferObject vbo2(60, GL_STATIC_DRAW);
    vao.EnableAttribute(0, vbo1, 1, GL_FLOAT, 1, 0);
    vao.EnableAttribute(3, 1, GL_FLOAT);
    BOOST_CHECK(vao.CheckVertexAttribsBoundToBuffer() == false);
    vao.SetAttributeBuffer(3, vbo1, 1, 60);
    vao.EnableAttribute(7, vbo2, 1, GL_FLOAT, 1, 0);
    BOOST_CHECK(vao.CheckVertexAttribsBoundToBuffer());

    BOOST_CHECK(vao.CheckAttributeEnabled(0));
    BOOST_CHECK(vao.CheckAttributeEnabled(3));
    BOOST_CHECK(vao.CheckAttributeEnabled(7));
    BOOST_CHECK(vao.CheckAttributeEnabled(1) == false);

    BOOST_CHECK(vao.QueryAttributeBuffer(0) == vbo1.GetHandle());
    BOOST_CHECK(vao.QueryAttributeBuffer(3) == vbo1.GetHandle());
    BOOST_CHECK(vao.QueryAttributeBuffer(7) == vbo2.GetHandle());
    BOOST_CHECK(vao.QueryAttributeBuffer(1) == 0);

    vao.Bind();
}
