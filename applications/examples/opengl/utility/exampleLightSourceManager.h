#pragma once

#include "gdl/math/mat4.h"
#include "gdl/math/vec3.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

#include "applications/examples/opengl/utility/meshGenerator.h"
#include "gdl/base/uniquePtr.h"

namespace GDL::OpenGL
{

class ExampleLightSourceManager
{
    VertexArrayObject mVAO;
    UniquePtr<BufferObject> mVBO;
    UniquePtr<BufferObject> mEBO;
    U32 mNumIndices = 0;
    Shader mVertexShader;
    Shader mFragmentShader;
    Program mProgram;
    GLint mUniformLightColor;
    GLint mUniformProjection;
    GLint mUniformLightPosition;
    GLint mUniformWorldCamera;

public:
    ExampleLightSourceManager();

    ExampleLightSourceManager(const ExampleLightSourceManager&) = delete;
    ExampleLightSourceManager(ExampleLightSourceManager&&) = delete;
    ExampleLightSourceManager& operator=(const ExampleLightSourceManager&) = delete;
    ExampleLightSourceManager& operator=(ExampleLightSourceManager&&) = delete;
    ~ExampleLightSourceManager() = default;

    void RenderPointLight(const Vec3f& lightPosition, const Vec3f& lightColor);

    void UpdateProjectionMatrix(const Mat4f& projectionMatrix);

    void UpdateCamera(const Mat4f& worldCameraMatrix);


private:

    void Initialize();


    const char* VertexShaderCode();


    const char* FragmentShaderCode();
};

} // namespace GDL::OpenGL


#include "applications/examples/opengl/utility/exampleLightSourceManager.inl"
