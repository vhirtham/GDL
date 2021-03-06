// GDL version of the sample code from
// OpenGL 4 Shading Language Cookbook - Second Edtion - Chapter 1 - Page 37:
#define TITLE "Sending data to a shader using uniform variables"
//
// Free link to the book:
// https://doc.lagout.org/programmation/OpenGL/OpenGL%204%20Shading%20Language%20Cookbook%20%282nd%20ed.%29%20%5BWolff%202013-12-24%5D.pdf


#include "gdl/base/time.h"
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
    // Setup Render Context #####################

    ContextManager& contextManager = ContextManager::Instance();
    RenderWindow renderWindow(contextManager);
    renderWindow.SetTitle(TITLE);
    renderWindow.Initialize();



    // Shader Code ##############################

    const char* vertexShaderCode = R"glsl(
            #version 430

            layout (location=0) in vec3 VertexPosition;
            layout (location=1) in vec3 VertexColor;

            out vec3 Color;

            uniform mat4 ProjectionMatrix;
            uniform mat4 ModelWorldMatrix;

            void main(void)
            {
                Color = VertexColor;

                gl_Position = ProjectionMatrix * ModelWorldMatrix * vec4(VertexPosition,1.0);
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



    // Create Shader and Program ################

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    Program program(vertexShader, fragmentShader);



    // Create Buffer Objects ####################
    // clang-format off
    Vector<F32> positionData
    {
        -1.0f, -0.6f,  0.0f,
         1.0f, -0.6f,  0.0f,
         0.0f,  1.0f,  0.0f
    };
    Vector<F32> colorData
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
    vao.EnableAttribute(0, 0, positionBuffer, GL_FLOAT, 3, 12);
    vao.EnableAttribute(1, 1, colorBuffer, GL_FLOAT, 3, 12);



    // Bind program and vertex array object #####
    vao.Bind();
    program.Use();



    // Get uniform location ####################

    GLint uniformProjection = program.QueryUniformLocation("ProjectionMatrix");
    GLint uniformModelWorld = program.QueryUniformLocation("ModelWorldMatrix");



    // Constant rotation setup ##################

    TimePoint startTime = CurrentTime();
    TimePoint prevTime = CurrentTime();
    TimePoint currTime = CurrentTime();

    F32 angle = 0.;
    F32 translateX = 0.;
    F32 translateY = 0.;

    // Uncomment for fullscreen #################
    // glutFullScreen();

    // Start main loop ##########################
    while (renderWindow.IsOpen())
    {
        contextManager.PollEvents();
        prevTime = currTime;
        currTime = CurrentTime();

        F32 frameTimeMS = std::chrono::duration_cast<Microseconds>(currTime - prevTime).count();
        F32 elapsedTimeMS = std::chrono::duration_cast<Microseconds>(currTime - startTime).count();

        angle += 3.14f * frameTimeMS / 5.e5f;
        translateX = std::sin(elapsedTimeMS / 3.e5f) * renderWindow.GetWidth() / 3.f;
        translateY = std::sin(elapsedTimeMS / 4.e5f) * renderWindow.GetHeight() / 3.f;

        Mat4f ProjectionMatrix =
                Transformations4::OrthogonalProjection(renderWindow.GetWidth(), renderWindow.GetHeight());
        Mat4f ModelWorldMatrix = Transformations4::Translation(translateX, translateY, 0.) *
                                 Transformations4::RotationZ(angle) * Transformations4::Scale(200.f, 200.f, 1);

        program.SetUniform(static_cast<I32>(uniformProjection), ProjectionMatrix);
        program.SetUniform(static_cast<I32>(uniformModelWorld), ModelWorldMatrix);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
