#include "gdl/base/timer.h"
#include "gdl/base/container/vector.h"
#include "gdl/math/mat4.h"
#include "gdl/math/transformations4.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/contextManager.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/renderWindow.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"


#include <chrono>
#include <thread>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace GDL;
using namespace GDL::OpenGL;



int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context ---------------------

    ContextManager& contextManager = ContextManager::Instance();
    RenderWindow renderWindow(contextManager);
    renderWindow.SetTitle("Indexed Drawing");
    renderWindow.Initialize();



    // Shader Code ------------------------------

    const char* vertexShaderCode = R"glsl(
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

    const char* fragmentShaderCode = R"glsl(
            #version 430

            in vec3 Color;

            out vec4 FragColor;

            void main(void)
            {
                FragColor = vec4(Color,1.0);
            }
            )glsl";



    // Create Shader and Program ----------------

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    Program program(vertexShader, fragmentShader);



    // Create Buffer Objects --------------------
    // clang-format off
    Vector<F32> positionData
    {
        -0.5f, -0.5f,  0.f,
        -0.5f,  0.5f,  0.f,
         0.5f,  0.5f,  0.f,
         0.5f, -0.5f,  0.f
    };

    Vector<F32> colorData
    {
         1.f,  0.f,  0.f,
         0.f,  1.f,  0.f,
         0.f,  0.f,  1.f,
         1.f,  0.f,  1.f
    };

    Vector<U32> indexData
    {
        0, 1, 2,
        2, 3, 0
    };
    // clang-format on

    BufferObject positionBuffer(positionData, GL_STATIC_DRAW);
    BufferObject colorBuffer(colorData, GL_STATIC_DRAW);
    BufferObject indexBuffer(indexData, GL_STATIC_DRAW);


    // Create Vertex Array Object ---------------
    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, positionBuffer, GL_FLOAT, 3, 12);
    vao.EnableAttribute(1, 1, colorBuffer, GL_FLOAT, 3, 12);
    vao.SetElementBuffer(indexBuffer);



    // Bind program and vertex array object -----
    vao.Bind();
    program.Use();


    // Other ------------------------------------
    Timer timer;
    bool wireframe = false;



    // Start main loop --------------------------
    while (renderWindow.IsOpen())
    {
        contextManager.PollEvents();

        if (timer.GetElapsedTime<Milliseconds>() > Milliseconds(1000))
        {
            if (wireframe)
                renderWindow.DisableWireframeMode();
            else
                renderWindow.EnableWireframeMode();
            wireframe = !wireframe;
            timer.Reset();
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
