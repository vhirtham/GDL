#include <boost/test/unit_test.hpp>

#include "gdl/base/Exception.h"
#include "gdl/GDLTypedefs.h"
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
    vao.AddAttribute(vbo1, 1, GL_FLOAT, 2, 0);
    vao.AddAttribute(vbo1, 1, GL_FLOAT, 2, 1);
    vao.AddAttribute(vbo1, 1, GL_FLOAT, 1, 80);
    vao.AddAttribute(vbo2, 1, GL_FLOAT, 1);
}
