

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


const char* VertexShaderCodeLightSource()
{
    return R"glsl(
            #version 450

            layout (location=0) in vec3 VertexPosition;
            layout (location=1) in vec2 vertexTexCoordinate;

            out vec2 texCoordinate;

            uniform vec4 lightPosition;
            uniform mat4 ProjectionMatrix;

            void main(void)
            {
                texCoordinate = vertexTexCoordinate;
                gl_Position = ProjectionMatrix * (vec4(VertexPosition + vec3(lightPosition), 1.0));
            }
            )glsl";
}


const char* FragmentShaderCodeLightSource()
{
    return R"glsl(
            #version 450

            uniform vec3 lightSourceIntensity;

            in vec2 texCoordinate;

            out vec4 FragColor;
            void main(void)
            {
                float x = texCoordinate.x * 2 - 1;
                float y = texCoordinate.y * 2 - 1;
                float r = sqrt((x * x + y * y));
                vec3 intensity = max((0.5 - r * r) * vec3(3,3,3),0);
                float alpha = max(2 - r * r - r ,0);
                FragColor = vec4(lightSourceIntensity + intensity, alpha);
            }
            )glsl";
}



// Main Program -------------------------------------------------------------------------------------------------------

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
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
    Shader vertexShaderTorus(GL_VERTEX_SHADER, VertexShaderCode());
    Shader fragmentShaderTorus(GL_FRAGMENT_SHADER, FragmentShaderCode());
    Program programTorus(diffuseLightIntensityShader, vertexShaderTorus, fragmentShaderTorus);

    Shader vertexShaderLightSource(GL_VERTEX_SHADER, VertexShaderCodeLightSource());
    Shader fragmentShaderLightSource(GL_FRAGMENT_SHADER, FragmentShaderCodeLightSource());
    Program programLightSource(vertexShaderLightSource, fragmentShaderLightSource);



    // Create Buffer Objects ---------------------------------------------

    auto [vertexDataTorus, elementDataTorus] = MeshGenerator::CreateTorus(1.f, 0.5f, 40, 40);
    BufferObject vertexBufferTorus(vertexDataTorus, GL_STATIC_DRAW);
    BufferObject elementBufferTorus(elementDataTorus, GL_STATIC_DRAW);

    auto [vertexDataLightSource, elementDataLightSource] = MeshGenerator::CreateRectangle<true>(0.5, 0.5);
    BufferObject vertexBufferLightSource(vertexDataLightSource, GL_STATIC_DRAW);
    BufferObject elementBufferLightSource(elementDataLightSource, GL_STATIC_DRAW);



    // Create Vertex Array Object ----------------------------------------

    VertexArrayObject vaoTorus;
    vaoTorus.EnableAttribute(0, 0, vertexBufferTorus, GL_FLOAT, 3, 24);
    vaoTorus.EnableAttribute(1, 0, GL_FLOAT, 3, 12);
    vaoTorus.SetElementBuffer(elementBufferTorus);

    VertexArrayObject vaoLightSource;
    vaoLightSource.EnableAttribute(0, 0, vertexBufferLightSource, GL_FLOAT, 3, 20);
    vaoLightSource.EnableAttribute(1, 0, GL_FLOAT, 2, 12);
    vaoLightSource.SetElementBuffer(elementBufferLightSource);



    // Get uniform locations ---------------------------------------------

    GLint uniformProjection = programTorus.QueryUniformLocation("ProjectionMatrix");
    GLint uniformRotation = programTorus.QueryUniformLocation("RotationMatrix");
    GLint uniformModelWorld = programTorus.QueryUniformLocation("ModelWorldMatrix");
    GLint uniformLightPosition = programTorus.QueryUniformLocation("lightPosition");
    GLint uniformLightSourceIntensity = programTorus.QueryUniformLocation("lightSourceIntensity");
    GLint uniformReflectionCoefficient = programTorus.QueryUniformLocation("reflectionCoefficient");

    GLint uniformProjectionLightSource = programLightSource.QueryUniformLocation("ProjectionMatrix");
    GLint uniformLightPositionLightSource = programLightSource.QueryUniformLocation("lightPosition");
    GLint uniformLightSourceIntensityLightSource = programLightSource.QueryUniformLocation("lightSourceIntensity");



    // Set light values --------------------------------------------------

    // Optimize this section as soon as vec3 and vec4 classes are implemented
    std::array<F32, 4> lightPositionData = {{0.f, 0.f, -10.f, 1.f}};
    std::array<F32, 3> lightSourceIntensityData = {{1.f, 0.1f, 0.4f}};
    std::array<F32, 3> reflectionCoefficientData = {{1.f, 1.f, 1.f}};

    glProgramUniform4fv(programTorus.GetHandle(), uniformLightPosition, 1, lightPositionData.data());
    glProgramUniform3fv(programTorus.GetHandle(), uniformLightSourceIntensity, 1, lightSourceIntensityData.data());
    glProgramUniform3fv(programTorus.GetHandle(), uniformReflectionCoefficient, 1, reflectionCoefficientData.data());

    glProgramUniform4fv(programLightSource.GetHandle(), uniformLightPositionLightSource, 1, lightPositionData.data());
    glProgramUniform3fv(programLightSource.GetHandle(), uniformLightSourceIntensityLightSource, 1,
                        lightSourceIntensityData.data());


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

        programTorus.SetUniform(uniformProjection, ProjectionMatrix);
        programTorus.SetUniform(uniformRotation, RotationMatrix);
        programTorus.SetUniform(uniformModelWorld, ModelWorldMatrix);
        programLightSource.SetUniform(uniformProjectionLightSource, ProjectionMatrix);

        vaoTorus.Bind();
        programTorus.Use();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(elementDataTorus.size()), GL_UNSIGNED_INT, nullptr);


        vaoLightSource.Bind();
        programLightSource.Use();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(elementDataLightSource.size()), GL_UNSIGNED_INT, nullptr);

        renderWindow.SwapBuffers();
    }


    exit(EXIT_SUCCESS);
}
