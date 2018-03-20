#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/programGL.h"
#include "gdl/rendering/openGL/shaderGL.h"
#include "gdl/rendering/openGL/renderWindowGL.h"

using namespace GDL;

RenderWindowGL& GetRenderWindow()
{
    static bool initialized = false;
    static RenderWindowGL renderWindow;
    if (!initialized)
    {
        renderWindow.Initialize();
        initialized = true;
    }
    return renderWindow;
}

BOOST_AUTO_TEST_CASE(LinkProgram)
{
    GetRenderWindow();

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

    ShaderGL vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    ShaderGL fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    ProgramGL program(vertexShader, fragmentShader);

    BOOST_CHECK_THROW(ProgramGL(vertexShader, vertexShader, fragmentShader), Exception);
    BOOST_CHECK_THROW(ProgramGL{vertexShader}, Exception);
    BOOST_CHECK_THROW(ProgramGL{fragmentShader}, Exception);
}


BOOST_AUTO_TEST_CASE(findInputs)
{
    GetRenderWindow();
}
