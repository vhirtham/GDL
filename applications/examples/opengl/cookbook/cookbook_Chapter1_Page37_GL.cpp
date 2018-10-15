// GDL version of the sample code from
// OpenGL 4 Shading Language Cookbook - Second Edtion - Chapter 1 - Page 37:
#define TITLE "Sending data to a shader using uniform variables"
//
// Free link to the book:
// https://doc.lagout.org/programmation/OpenGL/OpenGL%204%20Shading%20Language%20Cookbook%20%282nd%20ed.%29%20%5BWolff%202013-12-24%5D.pdf


#include "gdl/base/time.h"
#include "gdl/math/mat4.inl"
#include "gdl/rendering/openGL/core/bufferObjectGL.h"
#include "gdl/rendering/openGL/core/programGL.h"
#include "gdl/rendering/openGL/core/renderWindowGL.h"
#include "gdl/rendering/openGL/core/shaderGL.h"
#include "gdl/rendering/openGL/core/vertexArrayObjectGL.h"


#include <chrono>
#include <thread>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace GDL;
using namespace GDL::OpenGL;


static bool RunProgramm = true;

void Close()
{
    RunProgramm = false;
}

void ResizeFunction(int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}


void RenderFunction()
{

    glutSwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutPostRedisplay();
}



int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context #####################

    RenderWindow renderWindow(1000, 800, TITLE);
    renderWindow.Initialize();

    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Close);



    // Shader Code ##############################

    const char vertexShaderCode[] = R"glsl(
            #version 430

            layout (location=0) in vec3 VertexPosition;
            layout (location=1) in vec3 VertexColor;

            out vec3 Color;

            uniform mat4 RotationMatrix;

            void main(void)
            {
                Color = VertexColor;

                gl_Position = RotationMatrix * vec4(VertexPosition,1.0);
            }
            )glsl";

    const char fragmentShaderCode[] = R"glsl(
            #version 430

            in vec3 Color;

            out vec4 FragColor;
            void main(void)
            {
                FragColor = vec4(Color,1.0);
            }
            )glsl";



    // Create Shader and Program ################

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    Program program(vertexShader, fragmentShader);



    // Create Buffer Objects ####################
    // clang-format off
    std::vector<F32> positionData
    {
        -0.8f, -0.8f,  0.0f,
         0.8f, -0.8f,  0.0f,
         0.0f,  0.8f,  0.0f
    };
    std::vector<F32> colorData
    {
         1.f,  0.f,  0.f,
         0.f,  1.f,  0.f,
         0.f,  0.f,  1.f
    };
    // clang-format on
    BufferObject positionBuffer(positionData, GL_STATIC_DRAW);
    BufferObject colorBuffer(colorData, GL_STATIC_DRAW);



    // Create Vertex Array Object ###############
    VertexArrayObject vao;
    vao.EnableAttribute(0, positionBuffer, 3, GL_FLOAT, 12);
    vao.EnableAttribute(1, colorBuffer, 3, GL_FLOAT, 12);



    // Bind program and vertex array object #####
    vao.Bind();
    program.Use();



    // Get uniform location ####################
    GLuint uniformLocation = program.QueryUniformLocation("RotationMatrix");



    // Constant rotation setup ##################

    TimePoint prevTime = CurrentTime();
    TimePoint currTime = CurrentTime();

    F32 angle = 0.;

    // Uncomment for fullscreen #################
    // glutFullScreen();

    // Start main loop ##########################
    while (RunProgramm)
    {
        prevTime = currTime;
        currTime = CurrentTime();
        angle += 3.14 * std::chrono::duration_cast<Microseconds>(currTime - prevTime).count() / 1.e6;

        program.SetUniform(static_cast<I32>(uniformLocation), Mat4f::RotationMatrixZ(angle));

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glutMainLoopEvent();
    }


    exit(EXIT_SUCCESS);
}
