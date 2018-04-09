// GDL version of the sample code from
// OpenGL 4 Shading Language Cookbook - Second Edtion - Chapter 1 - Page 37:
#define TITLE "Sending data to a shader using uniform variables"
//
// Free link to the book:
// https://doc.lagout.org/programmation/OpenGL/OpenGL%204%20Shading%20Language%20Cookbook%20%282nd%20ed.%29%20%5BWolff%202013-12-24%5D.pdf


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


bool RunProgramm = true;

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



int main(int argc, char* argv[])
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
        -0.8, -0.8,  0.0,
         0.8, -0.8,  0.0,
         0.0,  0.8,  0.0
    };
    std::vector<F32> colorData
    {
         1.0,  0.0,  0.0,
         0.0,  1.0,  0.0,
         0.0,  0.0,  1.0
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

    using namespace std::chrono_literals;
    std::chrono::system_clock::time_point prevTime = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point currTime = std::chrono::system_clock::now();
    F32 angle = 0.;

    // Uncomment for fullscreen #################
    // glutFullScreen();

    // Start main loop ##########################
    while (RunProgramm)
    {
        prevTime = currTime;
        currTime = std::chrono::system_clock::now();
        angle += 3.14 * std::chrono::duration_cast<std::chrono::microseconds>(currTime - prevTime).count() / 1.e6;

        program.SetUniform(uniformLocation, Mat4f::RotationMatrixZ(angle));

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glutMainLoopEvent();
    }


    exit(EXIT_SUCCESS);
}