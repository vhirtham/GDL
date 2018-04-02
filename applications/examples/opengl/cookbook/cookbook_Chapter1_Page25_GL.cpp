// GDL version of the sample code from
// OpenGL 4 Shading Language Cookbook - Second Edtion - Chapter 1 - Page 25:
// "Sending data to a shader using vertex attributes and vertex buffer objects"



#include "gdl/rendering/openGL/core/bufferObjectGL.h"
#include "gdl/rendering/openGL/core/programGL.h"
#include "gdl/rendering/openGL/core/renderWindowGL.h"
#include "gdl/rendering/openGL/core/shaderGL.h"
#include "gdl/rendering/openGL/core/vertexArrayObjectGL.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace GDL;
using namespace GDL::OpenGL;

void ResizeFunction(int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}


void RenderFunction()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
    glutPostRedisplay();
}



int main(int argc, char* argv[])
{
    // Setup Render Context #####################

    RenderWindow renderWindow(1200, 800, "OpenGL 4 Shading Language Cookbook - Second Edtion - Chapter 1 - Page 25");
    renderWindow.Initialize();

    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);



    // Shader Code ##############################

    const char vertexShaderCode[] = R"glsl(
            #version 430

            layout (location=0) in vec3 VertexPosition;
            layout (location=1) in vec3 VertexColor;

            out vec3 Color;

            void main(void)
            {
                Color = VertexColor;

                gl_Position = vec4(VertexPosition,1.0);
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



    // Start main loop ##########################
    glutMainLoop();


    exit(EXIT_SUCCESS);
}
