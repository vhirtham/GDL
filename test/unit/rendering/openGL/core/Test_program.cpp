#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/rendering/openGL/core/contextGLUT.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;
using namespace GDL::OpenGL;

RenderWindowGLUT& GetRenderWindowGLUT()
{
    ContextGLUT& contextGLUT = ContextGLUT::Instance();
    static RenderWindowGLUT renderWindow(contextGLUT);
    if (!renderWindow.IsInitialized())
    {
        contextGLUT.EnableDebug();
        contextGLUT.GetDebugMessageHandler().SetOutputMethod(GL_DEBUG_TYPE_ERROR, OutputMethod::COUT);
        renderWindow.Initialize();
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
}



BOOST_AUTO_TEST_CASE(Uniforms)
{
    GetRenderWindowGLUT();

    const char vertexShaderCode[] = R"glsl(
                                    #version 430

                                    uniform mat4 transformationMat;

                                    layout(location=0) in vec4 in_Position;
                                    void main(void)
                                    {
                                        gl_Position = transformationMat * in_Position;
                                    }
                                    )glsl";


    const char fragmentShaderCode[] = R"glsl(
                                    #version 430

                                    uniform float x;

                                    out vec4 out_Color;
                                    void main(void)
                                    {
                                        out_Color = vec4(x,1.,1.,1.);
                                    }
                                    )glsl";

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    Program program(vertexShader, fragmentShader);

    GLint xLocation = program.QueryUniformLocation("x");
    GLint transformationMatLocation = program.QueryUniformLocation("transformationMat");

    program.SetUniform(xLocation, 2.f);
    program.SetUniform(transformationMatLocation, Mat4f());
}
