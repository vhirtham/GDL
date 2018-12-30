#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/timer.h"
#include "gdl/base/container/vector.h"
#include "gdl/math/transformationMatrix.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/contextManager.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/renderWindow.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

#include "applications/examples/opengl/utility/meshGenerator.h"
#include "applications/examples/opengl/utility/lightSourceVisualizer.h"

#include <cmath>

using namespace GDL;
using namespace GDL::OpenGL;


// Shader Code --------------------------------------------------------------------------------------------------------

const char* DiffuseLightIntensityShaderCode()
{
    return R"glsl(
            #version 450

            uniform vec4 lightPosition;
            uniform vec3 lightSourceIntensity;
            uniform vec3 reflectionCoefficient;

            vec3 DiffuseLightIntensity(in const vec4 surfaceWorldPosition, in vec3 normalWorld)
            {
                vec3 s = normalize(vec3(lightPosition - surfaceWorldPosition));
                return lightSourceIntensity * reflectionCoefficient * max(dot(s, normalWorld), 0.);
            }
            )glsl";
}



const char* VertexShaderCode()
{
    return R"glsl(
            #version 450

            layout (location=0) in vec3 VertexPosition;
            layout (location=1) in vec3 VertexNormal;

            uniform mat4 ProjectionMatrix;
            uniform mat4 RotationMatrix;
            uniform mat4 ModelWorldMatrix;

            out vec3 lightIntensity;

            // Forward declaration of lighting function
            vec3 DiffuseLightIntensity(in const vec4 surfaceWorldPosition, in vec3 normalWorld);

            void main(void)
            {
                mat3 normalMatrix = mat3(RotationMatrix);
                vec3 normalWorld = normalize(normalMatrix * VertexNormal);
                vec4 positionWorld = ModelWorldMatrix * vec4(VertexPosition, 1.0);

                lightIntensity = DiffuseLightIntensity(positionWorld, normalWorld);
                gl_Position = ProjectionMatrix * positionWorld;
            }
            )glsl";
}

const char* FragmentShaderCode()
{
    return R"glsl(
            #version 450

            in vec3 lightIntensity;
            out vec4 FragColor;

            void main(void)
            {
                FragColor = vec4(lightIntensity,1.0);
            }
            )glsl";
}



// Main Program -------------------------------------------------------------------------------------------------------

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Light source configuration ----------------------------------------

    // Optimize this section as soon as vec3 and vec4 classes are implemented
    std::array<F32, 4> lightPositionData = {{0.f, 0.f, -10.f, 1.f}};
    std::array<F32, 3> lightSourceIntensityData = {{1.f, 0.1f, 0.4f}};
    std::array<F32, 3> reflectionCoefficientData = {{1.f, 1.f, 1.f}};

    // Setup Render Context ----------------------------------------------

    ContextManager& contextManager = ContextManager::Instance();
    contextManager.EnableDebug();
    contextManager.Initialize();

    RenderWindow renderWindow(contextManager);
    renderWindow.SetTitle("Diffuse per vertex shading");
    renderWindow.Initialize();
    renderWindow.EnableDepthTest();

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    // Create Shader and Program -----------------------------------------

    Shader diffuseLightIntensityShader(GL_VERTEX_SHADER, DiffuseLightIntensityShaderCode());
    Shader vertexShader(GL_VERTEX_SHADER, VertexShaderCode());
    Shader fragmentShader(GL_FRAGMENT_SHADER, FragmentShaderCode());
    Program program(diffuseLightIntensityShader, vertexShader, fragmentShader);



    // Create Buffer Objects ---------------------------------------------

    auto [vertexData, elementData] = MeshGenerator::CreateTorus(1.f, 0.5f, 40, 40);
    BufferObject vertexBuffer(vertexData, GL_STATIC_DRAW);
    BufferObject elementBuffer(elementData, GL_STATIC_DRAW);



    // Create Vertex Array Object ----------------------------------------

    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, vertexBuffer, GL_FLOAT, 3, 24);
    vao.EnableAttribute(1, 0, GL_FLOAT, 3, 12);
    vao.SetElementBuffer(elementBuffer);



    // Create light source visualizer ------------------------------------
    LightSourceVisualizer lsv;



    // Get uniform locations ---------------------------------------------

    GLint uniformProjection = program.QueryUniformLocation("ProjectionMatrix");
    GLint uniformRotation = program.QueryUniformLocation("RotationMatrix");
    GLint uniformModelWorld = program.QueryUniformLocation("ModelWorldMatrix");
    GLint uniformLightPosition = program.QueryUniformLocation("lightPosition");
    GLint uniformLightSourceIntensity = program.QueryUniformLocation("lightSourceIntensity");
    GLint uniformReflectionCoefficient = program.QueryUniformLocation("reflectionCoefficient");



    // Set light values --------------------------------------------------

    glProgramUniform4fv(program.GetHandle(), uniformLightPosition, 1, lightPositionData.data());
    glProgramUniform3fv(program.GetHandle(), uniformLightSourceIntensity, 1, lightSourceIntensityData.data());
    glProgramUniform3fv(program.GetHandle(), uniformReflectionCoefficient, 1, reflectionCoefficientData.data());

    lsv.SetPosition(lightPositionData);
    lsv.SetIntensity(lightSourceIntensityData);



    // Create Timer ------------------------------------------------------
    Timer timer;



    // Start main loop ---------------------------------------------------
    while (renderWindow.IsOpen())
    {
        contextManager.PollEvents();

        F32 elapsedMS = timer.GetElapsedTime<Microseconds>().count();
        F32 angle = elapsedMS / 1.e6f;
        F32 x = -std::sin(angle) * 3.f;
        F32 y = -std::sin(angle / 3.f);
        F32 z = -std::cos(angle) * 3.f - 10.f;

        Mat4f ProjectionMatrix = TransformationMatrix4::PerspectiveProjection(65.f, renderWindow.GetWidth(),
                                                                              renderWindow.GetHeight(), 0.1f, 100.f);
        Mat4f RotationMatrix =
                TransformationMatrix4::RotationY(angle * 1.5f) * TransformationMatrix4::RotationX(angle * 2.f);
        Mat4f TranslationMatrix = TransformationMatrix4::Translation(x, y, z);
        Mat4f ModelWorldMatrix = TranslationMatrix * RotationMatrix;

        program.SetUniform(uniformProjection, ProjectionMatrix);
        program.SetUniform(uniformRotation, RotationMatrix);
        program.SetUniform(uniformModelWorld, ModelWorldMatrix);
        lsv.SetProjection(ProjectionMatrix);

        vao.Bind();
        program.Use();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(elementData.size()), GL_UNSIGNED_INT, nullptr);

        lsv.RenderLightSource();

        renderWindow.SwapBuffers();
    }


    exit(EXIT_SUCCESS);
}
