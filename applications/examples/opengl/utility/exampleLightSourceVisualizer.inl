#pragma once

#include "applications/examples/opengl/utility/exampleLightSourceVisualizer.h"


namespace GDL::OpenGL
{



ExampleLightSourceVisualizer::ExampleLightSourceVisualizer()
    : mVertexShader(GL_VERTEX_SHADER, GetVertexShaderCode())
    , mFragmentShader(GL_FRAGMENT_SHADER, GetFragmentShaderCode())
    , mProgram(mVertexShader, mFragmentShader)
    , mUniformLightColor(mProgram.QueryUniformLocation("lightColor"))
    , mUniformProjection(mProgram.QueryUniformLocation("projectionMatrix"))
    , mUniformLightPosition(mProgram.QueryUniformLocation("lightPosition"))
    , mUniformWorldCamera(mProgram.QueryUniformLocation("worldCameraMatrix"))
    , mUniformDiameter(mProgram.QueryUniformLocation("diameter"))
{
    Initialize();
}

void ExampleLightSourceVisualizer::RenderPointLight(const Vec3f& lightPosition, const Vec3f& lightColor, F32 diameter)
{
    mProgram.SetUniform(mUniformLightPosition, lightPosition);
    mProgram.SetUniform(mUniformLightColor, lightColor);
    mProgram.SetUniform(mUniformDiameter, diameter);

    mVAO.Bind();
    mProgram.Use();
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, nullptr);
}



void ExampleLightSourceVisualizer::Initialize()
{
    auto [vertexDataLightSource, elementDataLightSource] = MeshGenerator::CreateSphere<false, true>(32, 32, 0.5);
    mNumIndices = static_cast<U32>(elementDataLightSource.size());
    mVBO = MakeUnique<BufferObject>(vertexDataLightSource, GL_STATIC_DRAW);
    mEBO = MakeUnique<BufferObject>(elementDataLightSource, GL_STATIC_DRAW);
    mVAO.EnableAttribute(0, 0, *mVBO, GL_FLOAT, 3, 20);
    mVAO.EnableAttribute(1, 0, GL_FLOAT, 2, 12);
    mVAO.SetElementBuffer(*mEBO);
}



void ExampleLightSourceVisualizer::UpdateProjectionMatrix(const Mat4f& projectionMatrix)
{
    mProgram.SetUniform(mUniformProjection, projectionMatrix);
}



void ExampleLightSourceVisualizer::UpdateCamera(const Mat4f& worldCameraMatrix)
{
    mProgram.SetUniform(mUniformWorldCamera, worldCameraMatrix);
}



const char* ExampleLightSourceVisualizer::GetVertexShaderCode()
{
    return R"glsl(
            #version 450

            layout (location=0) in vec3 VertexPosition;
            layout (location=1) in vec2 vertexTexCoordinate;


            uniform vec3 lightPosition;
            uniform mat4 projectionMatrix;
            uniform mat4 worldCameraMatrix;
            uniform float diameter;

            void main(void)
            {
                gl_Position = projectionMatrix * worldCameraMatrix * (vec4(VertexPosition * diameter + lightPosition, 1.f));
            }
            )glsl";
}



const char* ExampleLightSourceVisualizer::GetFragmentShaderCode()
{
    return R"glsl(
            #version 450

            uniform vec3 lightColor;

            out vec4 FragColor;

            void main(void)
            {

                FragColor = vec4(lightColor, 1.f);
            }
            )glsl";
}



} // namespace GDL::OpenGL
