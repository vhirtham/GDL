#pragma once

#include "applications/examples/opengl/utility/examplePhongLightManager.h"



namespace GDL::OpenGL
{

ExamplePhongLightManager::ExamplePhongLightManager(Program& program)
    : mProgram{program}
{
    Initialize();
}



void ExamplePhongLightManager::AddPointLight(const Vec3f& position, const Vec3f& color, F32 attConst, F32 attLinear,
                                             F32 attQuadratic)
{
    AddPointLight(position, color, color, color, attConst, attLinear, attQuadratic);
}



void ExamplePhongLightManager::AddPointLight(const Vec3f& position, const Vec3f& ambient, const Vec3f& diffuse,
                                             const Vec3f& specular, F32 attConst, F32 attLinear, F32 attQuadratic)
{
    U32 index = static_cast<U32>(mPointLights.size());
    String uniformName = MakeString("pointLights[", ToString(index), "]");

    PointLight& pointLight = mPointLights.emplace_back();
    pointLight.mPosition = position;
    pointLight.mAmbient = ambient;
    pointLight.mDiffuse = diffuse;
    pointLight.mSpecular = specular;
    pointLight.mAttenuationConstant = attConst;
    pointLight.mAttenuationLinear = attLinear;
    pointLight.mAttenuationQuadratic = attQuadratic;

    pointLight.mUniformPosition = mProgram.QueryUniformLocation(MakeString(uniformName, ".position").c_str());
    pointLight.mUniformAmbient = mProgram.QueryUniformLocation(MakeString(uniformName, ".ambient").c_str());
    pointLight.mUniformDiffuse = mProgram.QueryUniformLocation(MakeString(uniformName, ".diffuse").c_str());
    pointLight.mUniformSpecular = mProgram.QueryUniformLocation(MakeString(uniformName, ".specular").c_str());
    pointLight.mUniformAttenuationConstant =
            mProgram.QueryUniformLocation(MakeString(uniformName, ".attenuationConstant").c_str());
    pointLight.mUniformAttenuationLinear =
            mProgram.QueryUniformLocation(MakeString(uniformName, ".attenuationLinear").c_str());
    pointLight.mUniformAttenuationQuadratic =
            mProgram.QueryUniformLocation(MakeString(uniformName, ".attenuationQuadratic").c_str());

    UpdatePointLight(index);
}



U32 ExamplePhongLightManager::GetNumPointLights() const
{
    return static_cast<U32>(mPointLights.size());
}



const Vec3f& ExamplePhongLightManager::GetPointLightDiffuse(U32 index) const
{
    return mPointLights[index].mDiffuse;
}



const Vec3f& ExamplePhongLightManager::GetPointLightPosition(U32 index) const
{
    return mPointLights[index].mPosition;
}

void ExamplePhongLightManager::SetDirectionalLight(const Vec3f& direction, const Vec3f& color)
{
    SetDirectionalLight(direction, color, color, color);
}



void ExamplePhongLightManager::SetDirectionalLight(const Vec3f& direction, const Vec3f& ambient, const Vec3f& diffuse,
                                                   const Vec3f& specular)
{
    mDirectionalLight.mDirection = direction;
    mDirectionalLight.mAmbient = ambient;
    mDirectionalLight.mDiffuse = diffuse;
    mDirectionalLight.mSpecular = specular;
    UpdateDirectionalLight();
}

void ExamplePhongLightManager::UpdateDirectionalLight()
{
    mProgram.SetUniform(mDirectionalLight.mUniformDirection, mDirectionalLight.mDirection);
    mProgram.SetUniform(mDirectionalLight.mUniformAmbient, mDirectionalLight.mAmbient);
    mProgram.SetUniform(mDirectionalLight.mUniformDiffuse, mDirectionalLight.mDiffuse);
    mProgram.SetUniform(mDirectionalLight.mUniformSpecular, mDirectionalLight.mSpecular);
}



void ExamplePhongLightManager::UpdatePointLight(U32 index)
{
    mProgram.SetUniform(mPointLights[index].mUniformPosition, mPointLights[index].mPosition);
    mProgram.SetUniform(mPointLights[index].mUniformAmbient, mPointLights[index].mAmbient);
    mProgram.SetUniform(mPointLights[index].mUniformDiffuse, mPointLights[index].mDiffuse);
    mProgram.SetUniform(mPointLights[index].mUniformSpecular, mPointLights[index].mSpecular);
    mProgram.SetUniform(mPointLights[index].mUniformAttenuationConstant, mPointLights[index].mAttenuationConstant);
    mProgram.SetUniform(mPointLights[index].mUniformAttenuationLinear, mPointLights[index].mAttenuationLinear);
    mProgram.SetUniform(mPointLights[index].mUniformAttenuationQuadratic, mPointLights[index].mAttenuationQuadratic);
}

void ExamplePhongLightManager::Initialize()
{
    mDirectionalLight.mUniformDirection = mProgram.QueryUniformLocation("directionalLight.direction");
    mDirectionalLight.mUniformAmbient = mProgram.QueryUniformLocation("directionalLight.ambient");
    mDirectionalLight.mUniformDiffuse = mProgram.QueryUniformLocation("directionalLight.diffuse");
    mDirectionalLight.mUniformSpecular = mProgram.QueryUniformLocation("directionalLight.specular");


}



const char* ExamplePhongLightManager::GetLightShaderCode()
{
    return R"glsl(
            #version 450

            struct DirectionalLight
            {
                vec3 direction;
                vec3 ambient;
                vec3 diffuse;
                vec3 specular;
            };


            struct PointLight
            {
                vec3 position;
                vec3 ambient;
                vec3 diffuse;
                vec3 specular;

                float attenuationConstant;
                float attenuationLinear;
                float attenuationQuadratic;
            };



            vec3 CalculateAmbient(in vec3 lightAmbient,
                                  in vec3 materialAlbedo)
            {
                return materialAlbedo * lightAmbient * 0.1;
            }



            vec3 CalculateDiffuse(in vec3 unitNormal,
                                  in vec3 lightDirection,
                                  in vec3 lightDiffuse,
                                  in vec3 materialAlbedo)
            {
                float diff = max(dot(unitNormal, lightDirection), 0.f);
                return materialAlbedo * lightDiffuse * diff;
            }



            vec3 CalculateSpecular(in vec3 fragmentPosition,
                                   in vec3 cameraPosition,
                                   in vec3 unitNormal,
                                   in vec3 lightDirection,
                                   in vec3 lightSpecular,
                                   in vec3 materialSpecular,
                                   in float materialReflectivity)
            {
                vec3 reflectionDirection = reflect(-lightDirection, unitNormal);
                vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);
                float spec = pow(max(dot(cameraDirection, reflectionDirection), 0.f), materialReflectivity);
                return materialSpecular * lightSpecular * spec;
            }



            vec3 CalculateDirectionalLight(in DirectionalLight directionalLight,
                                           in vec3 fragmentPosition,
                                           in vec3 cameraPosition,
                                           in vec3 unitNormal,
                                           in vec3 materialAlbedo,
                                           in vec3 materialSpecular,
                                           float materialReflectivity)
            {
                vec3 lightDirection = normalize(-directionalLight.direction);

                vec3 ambient = CalculateAmbient(directionalLight.ambient,
                                                materialAlbedo);

                vec3 diffuse = CalculateDiffuse(unitNormal,
                                                lightDirection,
                                                directionalLight.diffuse,
                                                materialAlbedo);

                vec3 specular = CalculateSpecular(fragmentPosition,
                                                  cameraPosition,
                                                  unitNormal,
                                                  lightDirection,
                                                  directionalLight.specular,
                                                  materialSpecular,
                                                  materialReflectivity);


                return ambient + diffuse + specular;
            }



            vec3 CalculatePointLight(in PointLight pointLight,
                                     in vec3 fragmentPosition,
                                     in vec3 cameraPosition,
                                     in vec3 unitNormal,
                                     in vec3 materialAlbedo,
                                     in vec3 materialSpecular,
                                     float materialReflectivity)
            {
                vec3 lightDirection = normalize(pointLight.position - fragmentPosition);
                float distance = length(pointLight.position - fragmentPosition);
                float attenuation = 1.f / (max(pointLight.attenuationConstant, 1.f) +
                                           pointLight.attenuationLinear * distance +
                                           pointLight.attenuationQuadratic * distance * distance);


                vec3 ambient = CalculateAmbient(pointLight.ambient,
                                                materialAlbedo);

                vec3 diffuse = CalculateDiffuse(unitNormal,
                                                lightDirection,
                                                pointLight.diffuse,
                                                materialAlbedo);

                vec3 specular = CalculateSpecular(fragmentPosition,
                                                  cameraPosition,
                                                  unitNormal,
                                                  lightDirection,
                                                  pointLight.specular,
                                                  materialSpecular,
                                                  materialReflectivity);


                return attenuation * (ambient + diffuse + specular);
            }



            #define NUM_POINT_LIGHTS 8
            uniform DirectionalLight directionalLight;
            uniform PointLight[NUM_POINT_LIGHTS] pointLights;



            vec3 CalculateLight(in vec3 fragmentPosition,
                                in vec3 cameraPosition,
                                in vec3 unitNormal,
                                in vec3 materialAlbedo,
                                in vec3 materialSpecular,
                                float materialReflectivity)
            {
                vec3 resultColor = CalculateDirectionalLight(directionalLight,
                                                             fragmentPosition,
                                                             cameraPosition,
                                                             unitNormal,
                                                             materialAlbedo,
                                                             materialSpecular,
                                                             materialReflectivity);

                for(int i = 0; i < NUM_POINT_LIGHTS; ++i)
                    resultColor += CalculatePointLight(pointLights[i],
                                                       fragmentPosition,
                                                       cameraPosition,
                                                       unitNormal,
                                                       materialAlbedo,
                                                       materialSpecular,
                                                       materialReflectivity);
                return resultColor;
            }


           )glsl";
}



} // namespace GDL::OpenGL
