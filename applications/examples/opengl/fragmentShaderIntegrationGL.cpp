
// Tutorials to go on: http://openglbook.com/chapter-1-getting-started.html

#include "gdl/base/time.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/contextGLUT.h"
#include "gdl/rendering/openGL/core/contextManagerGLFW.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"
#include "gdl/rendering/openGL/core/renderWindowGLFW.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"
#include "gdl/rendering/openGL/management/programDataManagerGL.h"

#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace GDL;
using namespace GDL::OpenGL;


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    ContextManagerGLFW& contextManager = ContextManagerGLFW::Instance();
    contextManager.EnableDebug();
    RenderWindowGLFW renderWindow(contextManager);
    renderWindow.SetTitle("OpenGL Triangle Test");
    renderWindow.Initialize();



    fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));
    const GLchar* VertexShaderCode = R"glsl(
                                    #version 430

                                     layout(location=0) in vec4 in_Position;
                                     layout(location=1) in vec4 in_Color;
                                     layout(location=2) in vec2 in_TexCoord;
                                     out vec4 ex_Color;
                                     out vec2 ex_TexCoord;

                                     void main(void)
                                     {
                                        gl_Position = in_Position;
                                        ex_Color = in_Color;
                                        ex_TexCoord = in_TexCoord;
                                     }
                                     )glsl";



    const GLchar* FragmentShaderCode = R"glsl(
            #version 430

            uniform float frequency;
            uniform float colMod[3];

            in vec4 ex_Color;
            in vec2 ex_TexCoord;
            out vec4 out_Color;

            void main(void)
            {
                vec4 color = vec4(ex_Color.x*colMod[0],ex_Color.y*colMod[1],ex_Color.z*colMod[2],ex_Color.w);
                float factor = 6.28 * frequency;
                float y = ex_TexCoord.y * factor;
                float dy = dFdy(ex_TexCoord.y) * 6.28 * factor;
                //"dy = clamp(dy,0.,6.28);
                float y1 = y - dy*0.5;
                float y2 = y + dy*0.5;
                float f1 = (sin(y)+1)*0.5;
                float f2 = (-cos(y2)+y2 + cos(y1)-y1)*0.5/dy;
                //out_Color = vec4(color.xyz,f1);
                out_Color = vec4(ex_Color.xyz,f2);
            }
            )glsl";


    Shader vertexShader(GL_VERTEX_SHADER, VertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, FragmentShaderCode);
    Program program(vertexShader, fragmentShader);


    // clang-format off
    Vector<F32> positionData{-0.8f, -1.0f, 0.0f, 1.0f,
                              0.0f,  1.0f, 0.0f, 1.0f,
                              0.8f, -1.0f, 0.0f, 1.0f};

    Vector<F32> colorData{1.0f, 0.0f, 0.0f, 1.0f,
                          0.0f, 1.0f, 0.0f, 1.0f,
                          0.0f, 1.0f, 1.0f, 0.0f};

    Vector<F32> texCoordData{0.0f, 0.0f,
                             0.5f, 1.0f,
                             1.0f, 0.0f};
    // clang-format on

    BufferObject positionBuffer(positionData, GL_STATIC_DRAW);
    BufferObject colorBuffer(colorData, GL_STATIC_DRAW);
    BufferObject texCoordBuffer(texCoordData, GL_STATIC_DRAW);

    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, positionBuffer, GL_FLOAT, 4, 16);
    vao.EnableAttribute(1, 1, positionBuffer, GL_FLOAT, 3, 12);
    vao.EnableAttribute(2, 2, texCoordBuffer, GL_FLOAT, 2, 8);



    vao.Bind();
    program.Use();
    GLint frequancyLocation = program.QueryUniformLocation("frequency");
    F32 frequency = 1.0;
    F32 factor = 1.;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    TimePoint prevTime = CurrentTime();
    TimePoint currTime = CurrentTime();


    while (renderWindow.IsOpen())
    {
        contextManager.PollEvents();

        prevTime = currTime;
        currTime = CurrentTime();
        F32 delta_t = std::chrono::duration_cast<Microseconds>(currTime - prevTime).count();


        frequency += factor * delta_t / (10.e3f * 2.f);
        if (frequency > 360.f)
        {
            factor = -1.0;
            glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
        }
        if (frequency < 1.f)
        {
            factor = 1.0;
            glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        }
        program.SetUniform(frequancyLocation, frequency);


        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (renderWindow.IsOpen())
            renderWindow.SwapBuffers();
    }
}
