#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/rendering/openGL/core/programGL.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"

using namespace GDL;
using namespace GDL::OpenGL;

RenderWindowGLUT& GetRenderWindowGLUT()
{
    static bool initialized = false;
    RenderWindowGLUT& renderWindow = RenderWindowGLUT::Instance();
    if (!initialized)
    {
        renderWindow.Initialize();
        initialized = true;
    }
    return renderWindow;
}

BOOST_AUTO_TEST_CASE(LinkProgram)
{
    GetRenderWindowGLUT();

    const char vertexShaderCode[] = R"glsl(
                                    #version 430
                                    layout(location=0) in vec4 in_Position;
                                    void main(void)
                                    {
                                        gl_Position = in_Position;
                                    }
                                    )glsl";


    const char fragmentShaderCode[] = R"glsl(
                                    #version 430
                                    out vec4 out_Color;
                                    void main(void)
                                    {
                                        out_Color = vec4(1.,1.,1.,1.);
                                    }
                                    )glsl";

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    Program program(vertexShader, fragmentShader);

    BOOST_CHECK_THROW(Program(vertexShader, vertexShader, fragmentShader), Exception);
    BOOST_CHECK_THROW(Program{vertexShader}, Exception);
    BOOST_CHECK_THROW(Program{fragmentShader}, Exception);
}
