#pragma once

#include "gdl/base/string.h"
#include "gdl/math/mat4.h"
#include "gdl/math/vec3.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

#include "applications/examples/opengl/utility/meshGenerator.h"



namespace GDL::OpenGL
{


class ExamplePhongLightManager
{
    struct DirectionalLight
    {
        Vec3f mDirection;
        Vec3f mAmbient;
        Vec3f mDiffuse;
        Vec3f mSpecular;

        GLint mUniformDirection;
        GLint mUniformAmbient;
        GLint mUniformDiffuse;
        GLint mUniformSpecular;
    };



    struct PointLight
    {
        Vec3f mPosition;
        Vec3f mAmbient;
        Vec3f mDiffuse;
        Vec3f mSpecular;
        F32 mAttenuationConstant;
        F32 mAttenuationLinear;
        F32 mAttenuationQuadratic;

        GLint mUniformPosition;
        GLint mUniformAmbient;
        GLint mUniformDiffuse;
        GLint mUniformSpecular;
        GLint mUniformAttenuationConstant;
        GLint mUniformAttenuationLinear;
        GLint mUniformAttenuationQuadratic;
    };



    Program& mProgram;
    DirectionalLight mDirectionalLight;
    Vector<PointLight> mPointLights;

public:
    ExamplePhongLightManager(Program& program);



    void AddPointLight(const Vec3f& position, const Vec3f& color, F32 attConst, F32 attLinear, F32 attQuadratic);

    void AddPointLight(const Vec3f& position, const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular,
                       F32 attConst, F32 attLinear, F32 attQuadratic);



    U32 GetNumPointLights() const;

    static const char* GetLightShaderCode();

    const Vec3f& GetPointLightDiffuse(U32 index) const;

    const Vec3f& GetPointLightPosition(U32 index) const;

    void SetDirectionalLight(const Vec3f& direction, const Vec3f& color);

    void SetDirectionalLight(const Vec3f& direction, const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular);


    void UpdateDirectionalLight();

    void UpdatePointLight(U32 index);

private:
    void Initialize();
};



} // namespace GDL::OpenGL


#include "applications/examples/opengl/utility/examplePhongLightManager.inl"
