#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/rendering/openGL/core/renderWindowGL.h"
#include "gdl/rendering/openGL/core/shaderGL.h"

using namespace GDL;
using namespace GDL::OpenGL;

BOOST_AUTO_TEST_CASE(Construction)
{
    RenderWindow& renderWindow = RenderWindow::Instance();
    renderWindow.Initialize();

    BOOST_CHECK_THROW(Shader shader(GL_VERTEX_SHADER, "fail"), Exception);

    const char vertexShaderCode[] = R"glsl(
                                    #version 430
                                    layout(location=0) in vec4 in_Position;
                                    void main(void)
                                    {
                                        gl_Position = in_Position;
                                    }
                                    )glsl";

    Shader shader2(GL_VERTEX_SHADER, vertexShaderCode);
}
