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


BOOST_AUTO_TEST_CASE(Inputs)
{
    GetRenderWindow();

    const char vertexShaderCode[] = R"glsl(
                                    #version 430
                                    layout(location=2) in vec2 in_TexCoord;
                                    layout(location=0) in vec4 in_Position;
                                    layout(location=1) in vec3 in_Color;

                                    void main(void)
                                    {
                                        gl_Position = vec4(in_Position.x,in_TexCoord.x,in_Color.x,1);
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

    BOOST_CHECK(program.GetNumInputs() == 3);
    BOOST_CHECK_THROW(program.GetInput("in_NotThere"), Exception);

    BOOST_CHECK_NO_THROW(program.GetInput("in_Position"));
    BOOST_CHECK(program.GetInput("in_Position").GetLocation() == 0);
    BOOST_CHECK(program.GetInput("in_Position").GetType() == GL_FLOAT_VEC4);

    BOOST_CHECK_NO_THROW(program.GetInput("in_Color"));
    BOOST_CHECK(program.GetInput("in_Color").GetLocation() == 1);
    BOOST_CHECK(program.GetInput("in_Color").GetType() == GL_FLOAT_VEC3);

    BOOST_CHECK_NO_THROW(program.GetInput("in_TexCoord"));
    BOOST_CHECK(program.GetInput("in_TexCoord").GetLocation() == 2);
    BOOST_CHECK(program.GetInput("in_TexCoord").GetType() == GL_FLOAT_VEC2);
}


BOOST_AUTO_TEST_CASE(Uniforms)
{
    GetRenderWindow();

    const char vertexShaderCode[] = R"glsl(
                                    #version 430

                                    uniform float X;
                                    layout (location = 0) uniform vec2 YZ;
                                    uniform float[2] w;

                                    void main(void)
                                    {
                                        gl_Position = vec4(X,YZ.x,YZ.y,w[0]+w[1]);
                                    }
                                    )glsl";


    const char fragmentShaderCode[] = R"glsl(
                                    #version 430
                                    out vec4 out_Color;

                                    layout (location = 0) uniform vec2 YZ;
                                    layout (location = 5) uniform vec4 a[3];

                                    void main(void)
                                    {
                                        out_Color = vec4(1.,YZ.x,1.,1.) + a[0]+ a[1]+ a[2];
                                    }
                                    )glsl";

    ShaderGL vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    ShaderGL fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    ProgramGL program(vertexShader, fragmentShader);

    // Check Getter #############################
    BOOST_CHECK(program.GetNumUniforms() == 7);
    BOOST_CHECK_THROW(program.GetUniform("not_there"), Exception);

    BOOST_CHECK_NO_THROW(program.GetUniform("X"));
    BOOST_CHECK(program.GetUniform("X").GetType() == GL_FLOAT);
    BOOST_CHECK(program.GetUniform("X").GetSubsequentArraySize() == 1);

    BOOST_CHECK_NO_THROW(program.GetUniform("YZ"));
    BOOST_CHECK(program.GetUniform("YZ").GetType() == GL_FLOAT_VEC2);
    BOOST_CHECK(program.GetUniform("YZ").GetSubsequentArraySize() == 1);
    BOOST_CHECK(program.GetUniform("YZ").GetLocation() == 0);

    BOOST_CHECK_THROW(program.GetUniform("w"), Exception);
    BOOST_CHECK_NO_THROW(program.GetUniform("w[0]"));
    BOOST_CHECK(program.GetUniform("w[0]").GetType() == GL_FLOAT);
    BOOST_CHECK(program.GetUniform("w[0]").GetSubsequentArraySize() == 2);
    BOOST_CHECK_NO_THROW(program.GetUniform("w[1]"));
    BOOST_CHECK(program.GetUniform("w[1]").GetType() == GL_FLOAT);
    BOOST_CHECK(program.GetUniform("w[1]").GetSubsequentArraySize() == 1);

    BOOST_CHECK_THROW(program.GetUniform("a"), Exception);
    BOOST_CHECK_NO_THROW(program.GetUniform("a[0]"));
    BOOST_CHECK(program.GetUniform("a[0]").GetType() == GL_FLOAT_VEC4);
    BOOST_CHECK(program.GetUniform("a[0]").GetSubsequentArraySize() == 3);
    BOOST_CHECK_NO_THROW(program.GetUniform("a[1]"));
    BOOST_CHECK(program.GetUniform("a[1]").GetType() == GL_FLOAT_VEC4);
    BOOST_CHECK(program.GetUniform("a[1]").GetSubsequentArraySize() == 2);
    BOOST_CHECK_NO_THROW(program.GetUniform("a[2]"));
    BOOST_CHECK(program.GetUniform("a[2]").GetType() == GL_FLOAT_VEC4);
    BOOST_CHECK(program.GetUniform("a[2]").GetSubsequentArraySize() == 1);


    // Check Setter #############################
    BOOST_CHECK_NO_THROW(program.SetUniformScalar("X", 1.f));
    BOOST_CHECK_NO_THROW(program.SetUniform<GL_FLOAT>("X", 1.f));
    BOOST_CHECK_NO_THROW(program.SetUniform<GL_FLOAT>(program.GetUniform("X").GetLocation(), 1.f));
    BOOST_CHECK_NO_THROW(program.SetUniformScalar(program.GetUniform("w[1]").GetLocation(), 1.f));
    BOOST_CHECK_NO_THROW(program.SetUniformScalarArray("w[0]", std::vector<F32>{{1.f, 2.f}}));
    BOOST_CHECK_NO_THROW(
            program.SetUniformScalarArray(program.GetUniform("w[0]").GetLocation(), std::vector<F32>{{1.f, 2.f}}));
}


BOOST_AUTO_TEST_CASE(UniformBlockss)
{
    GetRenderWindow();
    const char vertexShaderCode[] = R"glsl(
                                    #version 430

                                    layout(std140,binding=3) uniform GlobalVariables
                                    {
                                        mat4 Perspective;
                                        vec4 Translation;
                                        mat3 Rotation;
                                    };

                                    layout(std140,binding=1) uniform test
                                    {
                                        mat4 T1;
                                        vec4 T2;
                                        mat3 R3;
                                    };

                                    void main(void)
                                    {
                                        gl_Position = vec4(1.,1.,1.,1.);
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
    int a = 0;
}
