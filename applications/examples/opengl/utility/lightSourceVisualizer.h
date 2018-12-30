#pragma once

#include "gdl/math/mat4.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

#include "applications/examples/opengl/utility/meshGenerator.h"
#include "gdl/base/uniquePtr.h"

namespace GDL::OpenGL
{

class LightSourceVisualizer
{
    VertexArrayObject mVAO;
    UniquePtr<BufferObject> mVBO;
    UniquePtr<BufferObject> mEBO;
    Shader mVertexShader;
    Shader mFragmentShader;
    Program mProgram;
    GLint mUniformProjection;
    GLint mUniformPosition;
    GLint mUniformIntensity;

public:
    LightSourceVisualizer()
        : mVertexShader(GL_VERTEX_SHADER, VertexShaderCode())
        , mFragmentShader(GL_FRAGMENT_SHADER, FragmentShaderCode())
        , mProgram(mVertexShader, mFragmentShader)
        , mUniformProjection(mProgram.QueryUniformLocation("ProjectionMatrix"))
        , mUniformPosition(mProgram.QueryUniformLocation("lightPosition"))
        , mUniformIntensity(mProgram.QueryUniformLocation("lightSourceIntensity"))
    {
        Initialize();
    }

    LightSourceVisualizer(const LightSourceVisualizer&) = delete;
    LightSourceVisualizer(LightSourceVisualizer&&) = delete;
    LightSourceVisualizer& operator=(const LightSourceVisualizer&) = delete;
    LightSourceVisualizer& operator=(LightSourceVisualizer&&) = delete;
    ~LightSourceVisualizer() = default;

    void SetPosition(const std::array<F32, 4>& data)
    {
        glProgramUniform4fv(mProgram.GetHandle(), mUniformPosition, 1, data.data());
    }

    void SetIntensity(const std::array<F32, 3>& data)
    {
        glProgramUniform3fv(mProgram.GetHandle(), mUniformIntensity, 1, data.data());
    }

    void SetProjection(const Mat4f& matrix)
    {
        mProgram.SetUniform(mUniformProjection,matrix);
    }

    void RenderLightSource()
    {
        mVAO.Bind();
        mProgram.Use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

private:
    void Initialize()
    {
        auto [vertexDataLightSource, elementDataLightSource] = MeshGenerator::CreateRectangle<true>(0.5, 0.5);
        mVBO = MakeUnique<BufferObject>(vertexDataLightSource, GL_STATIC_DRAW);
        mEBO = MakeUnique<BufferObject>(elementDataLightSource, GL_STATIC_DRAW);
        mVAO.EnableAttribute(0, 0, *mVBO, GL_FLOAT, 3, 20);
        mVAO.EnableAttribute(1, 0, GL_FLOAT, 2, 12);
        mVAO.SetElementBuffer(*mEBO);
    }

    const char* VertexShaderCode()
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


    const char* FragmentShaderCode()
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
};

} // namespace GDL::OpenGL
