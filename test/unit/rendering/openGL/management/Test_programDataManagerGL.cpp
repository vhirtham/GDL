#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/management/programDataManagerGL.h"

#include "test/tools/GetRenderWindowGLUT.h"

using namespace GDL;
using namespace GDL::OpenGL;


BOOST_AUTO_TEST_CASE(Inputs)
{
    GetRenderWindowGLUT();

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

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    Program program(vertexShader, fragmentShader);
    ProgramDataManager programDM(program);

    BOOST_CHECK(programDM.GetNumInputs() == 3);
    BOOST_CHECK_THROW(programDM.GetInput("in_NotThere"), Exception);

    BOOST_CHECK_NO_THROW(programDM.GetInput("in_Position"));
    BOOST_CHECK(programDM.GetInput("in_Position").GetLocation() == 0);
    BOOST_CHECK(programDM.GetInput("in_Position").GetType() == GL_FLOAT_VEC4);

    BOOST_CHECK_NO_THROW(programDM.GetInput("in_Color"));
    BOOST_CHECK(programDM.GetInput("in_Color").GetLocation() == 1);
    BOOST_CHECK(programDM.GetInput("in_Color").GetType() == GL_FLOAT_VEC3);

    BOOST_CHECK_NO_THROW(programDM.GetInput("in_TexCoord"));
    BOOST_CHECK(programDM.GetInput("in_TexCoord").GetLocation() == 2);
    BOOST_CHECK(programDM.GetInput("in_TexCoord").GetType() == GL_FLOAT_VEC2);
}


BOOST_AUTO_TEST_CASE(Uniforms)
{
    GetRenderWindowGLUT();

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

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    Program program(vertexShader, fragmentShader);
    ProgramDataManager programDM(program);

    // Check Data and Getter ####################
    BOOST_CHECK(programDM.GetNumUniforms() == 7);
    BOOST_CHECK_THROW(programDM.GetUniform("not_there"), Exception);

    BOOST_CHECK_NO_THROW(programDM.GetUniform("X"));
    BOOST_CHECK(programDM.GetUniform("X").GetType() == GL_FLOAT);
    BOOST_CHECK(programDM.GetUniform("X").GetArraySize() == 1);

    BOOST_CHECK_NO_THROW(programDM.GetUniform("YZ"));
    BOOST_CHECK(programDM.GetUniform("YZ").GetType() == GL_FLOAT_VEC2);
    BOOST_CHECK(programDM.GetUniform("YZ").GetArraySize() == 1);
    BOOST_CHECK(programDM.GetUniform("YZ").GetLocation() == 0);

    BOOST_CHECK_THROW(programDM.GetUniform("w"), Exception);
    BOOST_CHECK_NO_THROW(programDM.GetUniform("w[0]"));
    BOOST_CHECK(programDM.GetUniform("w[0]").GetType() == GL_FLOAT);
    BOOST_CHECK(programDM.GetUniform("w[0]").GetArraySize() == 2);
    BOOST_CHECK_NO_THROW(programDM.GetUniform("w[1]"));
    BOOST_CHECK(programDM.GetUniform("w[1]").GetType() == GL_FLOAT);
    BOOST_CHECK(programDM.GetUniform("w[1]").GetArraySize() == 1);

    BOOST_CHECK_THROW(programDM.GetUniform("a"), Exception);
    BOOST_CHECK_NO_THROW(programDM.GetUniform("a[0]"));
    BOOST_CHECK(programDM.GetUniform("a[0]").GetType() == GL_FLOAT_VEC4);
    BOOST_CHECK(programDM.GetUniform("a[0]").GetArraySize() == 3);
    BOOST_CHECK_NO_THROW(programDM.GetUniform("a[1]"));
    BOOST_CHECK(programDM.GetUniform("a[1]").GetType() == GL_FLOAT_VEC4);
    BOOST_CHECK(programDM.GetUniform("a[1]").GetArraySize() == 2);
    BOOST_CHECK_NO_THROW(programDM.GetUniform("a[2]"));
    BOOST_CHECK(programDM.GetUniform("a[2]").GetType() == GL_FLOAT_VEC4);
    BOOST_CHECK(programDM.GetUniform("a[2]").GetArraySize() == 1);



    // Check Setter #############################

    // make this a template function when checking multiple types
    std::vector<F32> valueVector{{1.f, 2.f}};
    std::array<F32, 2> valueArray{{1.f, 2.f}};
    BOOST_CHECK_NO_THROW(programDM.SetUniform<GL_FLOAT>("X", 1.f));
    BOOST_CHECK_NO_THROW(programDM.SetUniform<GL_FLOAT>(programDM.GetUniform("X").GetLocation(), 1.f));
    BOOST_CHECK_NO_THROW(programDM.SetUniform<GL_FLOAT>(programDM.GetUniform("w[1]").GetLocation(), 1.f));
    BOOST_CHECK_NO_THROW(programDM.SetUniformArray<GL_FLOAT>("w[0]", valueVector.data(), valueVector.size()));
    BOOST_CHECK_NO_THROW(programDM.SetUniformArray<GL_FLOAT>(programDM.GetUniform("w[0]").GetLocation(),
                                                             valueVector.data(), valueVector.size()));
    BOOST_CHECK_NO_THROW(programDM.SetUniformArray<GL_FLOAT>("w[0]", valueArray.data(), valueArray.size()));
    BOOST_CHECK_NO_THROW(programDM.SetUniformArray<GL_FLOAT>(programDM.GetUniform("w[0]").GetLocation(),
                                                             valueArray.data(), valueArray.size()));
}



BOOST_AUTO_TEST_CASE(UniformBlocks)
{
    GetRenderWindowGLUT();
    const char vertexShaderCode[] = R"glsl(
                                    #version 430

                                    layout(std140,binding=3) uniform GlobalVariables
                                    {
                                        mat4 Perspective;
                                        vec4 Translation;
                                        mat3 Rotation;
                                    };

                                    layout(std140,binding=1) uniform FrameVariables
                                    {
                                        float T1;
                                        vec2 T2;
                                        mat2 R3;
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

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    Program program(vertexShader, fragmentShader);
    ProgramDataManager programDM(program);

    BOOST_CHECK(programDM.GetNumUniformBlocks() == 2);
    BOOST_CHECK_NO_THROW(programDM.GetUniformBlock("GlobalVariables"));
    BOOST_CHECK_NO_THROW(programDM.GetUniformBlock("FrameVariables"));
    BOOST_CHECK_THROW(programDM.GetUniformBlock("No_Variables"), Exception);

    const UniformBlock& globalVariables = programDM.GetUniformBlock("GlobalVariables");
    BOOST_CHECK(globalVariables.GetBindingPoint() == 3);
    BOOST_CHECK(globalVariables.GetNumVariables() == 3);
    BOOST_CHECK(globalVariables.GetIndex() < programDM.GetNumUniformBlocks());
    BOOST_CHECK(globalVariables.GetSize() == 128);

    const UniformBlock& frameVariables = programDM.GetUniformBlock("FrameVariables");
    BOOST_CHECK(frameVariables.GetBindingPoint() == 1);
    BOOST_CHECK(frameVariables.GetNumVariables() == 3);
    BOOST_CHECK(frameVariables.GetIndex() < programDM.GetNumUniformBlocks());
    BOOST_CHECK(frameVariables.GetSize() == 48);

    BOOST_CHECK(frameVariables.GetIndex() != globalVariables.GetIndex());

    BOOST_CHECK_NO_THROW(globalVariables.GetVariable("Perspective"));
    BOOST_CHECK_NO_THROW(globalVariables.GetVariable("Translation"));
    BOOST_CHECK_NO_THROW(globalVariables.GetVariable("Rotation"));
    BOOST_CHECK_THROW(globalVariables.GetVariable("NotThere"), Exception);

    const UniformBlockVariable& Perspective = globalVariables.GetVariable("Perspective");
    BOOST_CHECK(Perspective.GetArraySize() == 1);
    BOOST_CHECK(Perspective.GetOffset() == 0);
    BOOST_CHECK(Perspective.GetType() == GL_FLOAT_MAT4);

    const UniformBlockVariable& Translation = globalVariables.GetVariable("Translation");
    BOOST_CHECK(Translation.GetArraySize() == 1);
    BOOST_CHECK(Translation.GetOffset() == 64);
    BOOST_CHECK(Translation.GetType() == GL_FLOAT_VEC4);

    const UniformBlockVariable& Rotation = globalVariables.GetVariable("Rotation");
    BOOST_CHECK(Rotation.GetArraySize() == 1);
    BOOST_CHECK(Rotation.GetOffset() == 80);
    BOOST_CHECK(Rotation.GetType() == GL_FLOAT_MAT3);

    BOOST_CHECK(Perspective.GetIndex() != Translation.GetIndex());
    BOOST_CHECK(Perspective.GetIndex() != Rotation.GetIndex());
    BOOST_CHECK(Rotation.GetIndex() != Translation.GetIndex());
}


BOOST_AUTO_TEST_CASE(Uniform_Block_Variable)
{
    UniformBlockVariable Ref(1, 2, GL_FLOAT, 3);
    UniformBlockVariable IndexDiff(2, 2, GL_FLOAT, 3);
    UniformBlockVariable OffsetDiff(1, 1, GL_FLOAT, 3);
    UniformBlockVariable TypeDiff(1, 2, GL_INT, 3);
    UniformBlockVariable ArraySizeDiff(1, 2, GL_FLOAT, 4);

    BOOST_CHECK(Ref.CheckEqualDataStructure(Ref));
    BOOST_CHECK(Ref.CheckEqualDataStructure(IndexDiff));
    BOOST_CHECK(Ref.CheckEqualDataStructure(OffsetDiff) == false);
    BOOST_CHECK(Ref.CheckEqualDataStructure(TypeDiff) == false);
    BOOST_CHECK(Ref.CheckEqualDataStructure(ArraySizeDiff) == false);
}
