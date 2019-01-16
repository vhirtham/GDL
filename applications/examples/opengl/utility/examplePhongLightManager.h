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

//! @brief Phong light model class that can currently handle 1 directional light, 8 point lights and 1 spotlight.
class ExamplePhongLightManager
{
    //! @brief Helper struct for directional lights
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


    //! @brief Helper struct for spotlights
    struct Spotlight
    {
        Vec3f mPosition;
        Vec3f mDirection;
        Vec3f mAmbient;
        Vec3f mDiffuse;
        Vec3f mSpecular;
        F32 mInnerCutOffCos;
        F32 mOuterCutOffCos;

        GLint mUniformPosition;
        GLint mUniformDirection;
        GLint mUniformAmbient;
        GLint mUniformDiffuse;
        GLint mUniformSpecular;
        GLint mUniformInnerCutOffCos;
        GLint mUniformOuterCutOffCos;
    };

    //! @brief Helper struct for point lights
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
    Spotlight mSpotlight;
    Vector<PointLight> mPointLights;

public:
    ExamplePhongLightManager() = delete;
    ExamplePhongLightManager(const ExamplePhongLightManager&) = delete;
    ExamplePhongLightManager(ExamplePhongLightManager&&) = delete;
    ExamplePhongLightManager& operator=(const ExamplePhongLightManager&) = delete;
    ExamplePhongLightManager& operator=(ExamplePhongLightManager&&) = delete;
    ~ExamplePhongLightManager() = default;

    //! @brief ctor
    //! @param program: Program that is used to render the scene
    ExamplePhongLightManager(Program& program);

    //! @brief Adds a pointlight
    //! @param position: Position of the point light
    //! @param color: Color of the point light
    //! @param attConst: Constant attenuation coefficient
    //! @param attLinear: Linear attenuation coefficient
    //! @param attQuadratic: Quadratic attenuation coefficient
    void AddPointLight(const Vec3f& position, const Vec3f& color, F32 attConst, F32 attLinear, F32 attQuadratic);

    //! @brief Adds a pointlight
    //! @param position: Position of the point light
    //! @param ambient: Ambient light color of the point light
    //! @param diffuse: Diffuse light color of the point light
    //! @param specular: Specular light color of the point light
    //! @param attConst: Constant attenuation coefficient
    //! @param attLinear: Linear attenuation coefficient
    //! @param attQuadratic: Quadratic attenuation coefficient
    void AddPointLight(const Vec3f& position, const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular,
                       F32 attConst, F32 attLinear, F32 attQuadratic);

    //! @brief Returns the number of point lights
    //! @return Number of point lights
    U32 GetNumPointLights() const;

    //! @brief Gets the shader code that contains all the phong lighting functions
    //! @return Shader code
    static const char* GetLightShaderCode();

    //! @brief Gets the point light diffuse color
    //! @param index: Index of the point light
    //! @return Point light diffuse color
    const Vec3f& GetPointLightDiffuse(U32 index) const;

    //! @brief Gets the point lights position
    //! @param index: Index of the point light
    //! @return Point light position
    const Vec3f& GetPointLightPosition(U32 index) const;

    //! @brief Sets the directional light
    //! @param direction: Direction of the directional light
    //! @param color: Color of the directional light
    void SetDirectionalLight(const Vec3f& direction, const Vec3f& color);

    //! @brief Sets the directional light
    //! @param direction: Direction of the directional light
    //! @param ambient: Ambient light color of the directional light
    //! @param diffuse: Diffuse light color of the directional light
    //! @param specular: Specular light color of the directional light
    void SetDirectionalLight(const Vec3f& direction, const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular);

    //! @brief Sets the spotlight
    //! @param position: Position of the spotlight
    //! @param direction: Direction of the spotlight
    //! @param color: Color of the spotlight
    //! @param innerCutOff: Angle of the inner cone
    //! @param outerCutOff: Angle of the outer cone
    void SetSpotlight(const Vec3f& position, const Vec3f& direction, const Vec3f& color, F32 innerCutOff,
                      F32 outerCutOff);

    //! @brief Sets the spotlight
    //! @param position: Position of the spotlight
    //! @param direction: Direction of the spotlight
    //! @param ambient: Ambient light color of the spotlight
    //! @param diffuse: Diffuse light color of the spotlight
    //! @param specular: Specular light color of the spotlight
    //! @param innerCutOff: Angle of the inner cone
    //! @param outerCutOff: Angle of the outer cone
    void SetSpotlight(const Vec3f& position, const Vec3f& direction, const Vec3f& ambient, const Vec3f& diffuse,
                      const Vec3f& specular, F32 innerCutOff, F32 outerCutOff);

    //! @brief Sets the spotlights color
    //! @param color: New color
    void SetSpotlightColor(const Vec3f& color);

    //! @brief Sets the spotlights direction
    //! @param direction: New direction
    void SetSpotlightDirection(const Vec3f& direction);

    //! @brief Sets the spotlights position
    //! @param position: New position
    void SetSpotlightPosition(const Vec3f& position);

    //! @brief Updates all lights in the program passed during construction
    void UpdateLights();

    //! @brief Updates the directional light in the program passed during construction
    void UpdateDirectionalLight();

    //! @brief Updates a point light in the program passed during construction
    //! @param index: Index of the point light
    void UpdatePointLight(U32 index);

    //! @brief Updates the spotlight in the program passed during construction
    void UpdateSpotlight();

private:
    //! @brief Initializes the class
    void Initialize();
};



} // namespace GDL::OpenGL


#include "applications/examples/opengl/utility/examplePhongLightManager.inl"
