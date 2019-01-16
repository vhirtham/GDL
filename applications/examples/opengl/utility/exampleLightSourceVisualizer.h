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


//! @brief Visualizes light sources
class ExampleLightSourceVisualizer
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
    GLint mUniformDiameter;

public:
    ExampleLightSourceVisualizer();
    ExampleLightSourceVisualizer(const ExampleLightSourceVisualizer&) = delete;
    ExampleLightSourceVisualizer(ExampleLightSourceVisualizer&&) = delete;
    ExampleLightSourceVisualizer& operator=(const ExampleLightSourceVisualizer&) = delete;
    ExampleLightSourceVisualizer& operator=(ExampleLightSourceVisualizer&&) = delete;
    ~ExampleLightSourceVisualizer() = default;

    //! @brief Renders a point light
    //! @param lightPosition: Position of the light
    //! @param lightColor: Color of the light
    //! @param diameter: Diameter of the rendered sphere
    //! @remark Make sure that the projection matrix and the world-to-camera-space matrix are up to date
    void RenderPointLight(const Vec3f& lightPosition, const Vec3f& lightColor, F32 diameter = 0.1);

    //! @brief Updates the projection matrix
    //! @param projectionMatrix: New projection matrix
    void UpdateProjectionMatrix(const Mat4f& projectionMatrix);

    //! @brief Updates the world-to-camera-space matrix
    //! @param worldCameraMatrix: New world-to-camera-space matrix
    void UpdateCamera(const Mat4f& worldCameraMatrix);


private:
    //! @brief Initializes the calss
    void Initialize();

    //! @brief Returns the vertex shader code of the internal render program
    //! @return Vertex shader code
    const char* GetVertexShaderCode();

    //! @brief Returns the fragment shader code of the internal render program
    //! @return Fragment shader code
    const char* GetFragmentShaderCode();
};



} // namespace GDL::OpenGL


#include "applications/examples/opengl/utility/exampleLightSourceVisualizer.inl"
