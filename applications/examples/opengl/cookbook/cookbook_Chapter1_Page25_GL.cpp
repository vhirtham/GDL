// GDL version of the sample code from
// OpenGL 4 Shading Language Cookbook - Second Edtion - Chapter 1 - Page 25:
#define TITLE "Sending data to a shader using vertex attributes and vertex buffer objects"
//
// Free link to the book:
// https://doc.lagout.org/programmation/OpenGL/OpenGL%204%20Shading%20Language%20Cookbook%20%282nd%20ed.%29%20%5BWolff%202013-12-24%5D.pdf


#include "gdl/base/container/vector.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/contextManager.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/renderWindow.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace GDL;
using namespace GDL::OpenGL;



int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context #####################

    ContextManager& contextManager = ContextManager::Instance();
    RenderWindow renderWindow(contextManager);
    renderWindow.SetTitle(TITLE);
    renderWindow.Initialize();



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
    Vector<F32> positionData
    {
        -0.8f, -0.8f,  0.0f,
         0.8f, -0.8f,  0.0f,
         0.0f,  0.8f,  0.0f
    };
    Vector<F32> colorData
    {
         1.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  0.0f,  1.0f
    };
    // clang-format on
    BufferObject positionBuffer(positionData, GL_STATIC_DRAW);
    BufferObject colorBuffer(colorData, GL_STATIC_DRAW);



    // Create Vertex Array Object ###############
    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, positionBuffer, GL_FLOAT, 3, 12);
    vao.EnableAttribute(1, 1, colorBuffer, GL_FLOAT, 3, 12);



    // Bind program and vertex array object #####
    vao.Bind();
    program.Use();



    // Start main loop ##########################
    while (renderWindow.IsOpen())
    {
        contextManager.PollEvents();

        glDrawArrays(GL_TRIANGLES, 0, 3);

        renderWindow.SwapBuffers();
    }


    exit(EXIT_SUCCESS);
}
