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

    const char vertexShaderCode[] = R"glsl(
                                    #version 430
                                    layout(location=2) in vec2 in_TexCoord;
                                    layout(location=0) in vec4 in_Position;
                                    layout(location=1) in vec3 in_Color;

                                    out vec3 Color;
                                    out vec2 TexCoord;
                                    void main(void)
                                    {
                                        gl_Position = in_Position;
                                        Color = in_Color;
                                        TexCoord = in_TexCoord;
                                    }
                                    )glsl";


    const char fragmentShaderCode[] = R"glsl(
                                    #version 430
                                    in vec3 Color;
                                    in vec2 TexCoord;
                                    out vec4 out_Color;
                                    void main(void)
                                    {
                                        out_Color = vec4(Color,TexCoord.x);
                                    }
                                    )glsl";

    ShaderGL vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    ShaderGL fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    ProgramGL program(vertexShader, fragmentShader);

    BOOST_CHECK(program.GetNumInputs() == 3);

    ProgramInput inputPosition = program.GetInput("in_Position");
    BOOST_CHECK(inputPosition.GetHandle() == 0);
    BOOST_CHECK(inputPosition.GetType() == GL_FLOAT_VEC4);
    ProgramInput inputColor = program.GetInput("in_Color");
    BOOST_CHECK(inputColor.GetHandle() == 1);
    BOOST_CHECK(inputColor.GetType() == GL_FLOAT_VEC3);
    ProgramInput inputTexCoord = program.GetInput("in_TexCoord");
    BOOST_CHECK(inputTexCoord.GetHandle() == 2);
    BOOST_CHECK(inputTexCoord.GetType() == GL_FLOAT_VEC2);

    BOOST_CHECK_THROW(program.GetInput("in_NotThere"), Exception);
}
