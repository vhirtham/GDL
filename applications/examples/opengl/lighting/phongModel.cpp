#include "gdl/base/timer.h"
#include "gdl/input/inputState.h"
#include "gdl/input/key.h"
#include "gdl/math/vec3.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"


#include "applications/examples/opengl/utility/defaultRenderSetup.h"
#include "applications/examples/opengl/utility/exampleSceneGenerator.h"
#include "applications/examples/opengl/utility/exampleCamera.h"
#include "applications/examples/opengl/utility/examplePhongLightManager.h"
#include "applications/examples/opengl/utility/exampleLightSourceVisualizer.h"


using namespace GDL;
using namespace GDL::Input;
using namespace GDL::OpenGL;
using namespace GDL::Transformations4;



// ShaderCode ---------------------------------------------------------------------------------------------------------

const char* GetVertexShaderCode()
{
    return R"glsl(
            #version 450

            layout (location=0) in vec3 vertexPosition;
            layout (location=1) in vec3 vertexNormal;
            layout (location=2) in vec2 vertexTexCoord;

            uniform mat4 projection;
            uniform mat4 modelWorld;
            uniform mat4 worldCamera;
            uniform vec2 textureScale;
            uniform vec2 textureOffset;

            out vec3 positionWorld;
            out vec3 normal;
            out vec2 texCoord;

            void main(void)
            {
                // Position Calculation
                vec4 positionWorld4 = modelWorld * vec4(vertexPosition, 1.f);
                positionWorld = vec3(positionWorld4);
                gl_Position =  projection * worldCamera * positionWorld4;


                // Normal calculation
                normal = mat3(transpose(inverse(modelWorld))) * vertexNormal;


                // Texture coordinate calculation
                texCoord = (vertexTexCoord + textureOffset) * textureScale;
            }
            )glsl";
}



const char* GetFragmentShaderCode()
{
    return R"glsl(
            #version 450

            struct Material
            {
                sampler2D albedo;
                sampler2D specular;
                float reflectivity;
            };


            uniform vec4 cameraPosition;
            uniform Material material;

            in vec3 positionWorld;
            in vec3 normal;
            in vec2 texCoord;

            out vec4 fragColor;


            vec3 CalculateLight(vec3, vec3, vec3, vec3, vec3, float);


            void main(void)
            {
                vec3 unitNormal = normalize(normal);
                vec4 materialAlbedo = texture(material.albedo, texCoord);
                vec4 materialSpecular = texture(material.specular, texCoord);

                vec3 resultColor = CalculateLight(positionWorld,
                                                  cameraPosition.xyz,
                                                  unitNormal,
                                                  materialAlbedo.xyz,
                                                  materialSpecular.xyz,
                                                  material.reflectivity);


                fragColor = vec4(resultColor, materialAlbedo.w);
            }
            )glsl";
}



// Main ---------------------------------------------------------------------------------------------------------------

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context ----------------------------------------------

    auto [renderWindow, contextManager] = DefaultRenderSetup();
    renderWindow.SetTitle("movement: W,A,S,D - flashlight: F - capture mouse: C - release mouse: ESC");



    // Create Shader and Program -----------------------------------------

    Shader vertexShader(GL_VERTEX_SHADER, GetVertexShaderCode());
    Shader lightShader(GL_FRAGMENT_SHADER, ExamplePhongLightManager::GetLightShaderCode());
    Shader fragmentShader(GL_FRAGMENT_SHADER, GetFragmentShaderCode());
    Program program(vertexShader, lightShader, fragmentShader);



    // Set uniform sampler texture units ---------------------------------

    program.SetUniformSamplerTextureUnit("material.albedo", 0);
    program.SetUniformSamplerTextureUnit("material.specular", 1);



    // Create default scene ----------------------------------------------

    ExampleSceneGenerator scene(program);
    scene.CreateExampleScene01();



    // Light source ------------------------------------------------------

    ExamplePhongLightManager phongLightManager(program);

    Vec3f ambientColor(0.025f, 0.025f, 0.125f);
    phongLightManager.SetDirectionalLight(Vec3f(-2.f, 2.f, -2.f), ambientColor, ambientColor, ambientColor);
    phongLightManager.AddPointLight(Vec3f(2, 6, 2), Vec3f(1, 1, 1), 1.f, 0.22f, 0.2f);
    phongLightManager.AddPointLight(Vec3f(-7, 8, 2.5), Vec3f(1.f, 0.1f, 0.1f), 1.f, 0.22f, 0.2f);
    phongLightManager.SetSpotlight(Vec3f(0, 0, 1.5), Vec3f(0.f, 1.f, -0.1f), Vec3f(1.f, 1.f, 1.f),
                                   4.5f * PI<F32> / 180.f, 10.f * PI<F32> / 180.f);



    // Camera ------------------------------------------------------------

    ExampleCamera camera(0, -5, 1.5);
    camera.SetMovementSpeed(2);
    GLint worldCameraLocation = program.QueryUniformLocation("worldCamera");



    // Create light source manager ---------------------------------------

    bool flashLightOn = false;
    ExampleLightSourceVisualizer lightSourceVisualizer;



    // Misc --------------------------------------------------------------
    bool prefKeyStateF = false;


    // Start main loop ---------------------------------------------------

    Timer timer;
    while (renderWindow.IsOpen())
    {
        F32 deltaTime = timer.GetElapsedTime<Microseconds>().count() / 1000000.f;
        timer.Reset();


        contextManager.PollEvents();
        if (InputState::GetKeyPressed(Key::ESC))
            renderWindow.ReleaseCursor();
        if (InputState::GetKeyPressed(Key::C))
            renderWindow.CaptureCursor();
        if (InputState::GetKeyPressed(Key::F))
        {
            if (!prefKeyStateF)
                flashLightOn = !flashLightOn;
        }
        prefKeyStateF = InputState::GetKeyPressed(Key::F);

        // Camera movement
        camera.MoveCamera(deltaTime);
        program.SetUniform(program.QueryUniformLocation("cameraPosition"), camera.GetCameraPosition());
        program.SetUniform(worldCameraLocation, camera.GetWorldToCamMatrix());


        // Update projection
        Mat4f projectionMatrix = PerspectiveProjection(45.f, static_cast<F32>(renderWindow.GetWidth()),
                                                       static_cast<F32>(renderWindow.GetHeight()), 0.1f, 100.f);
        lightSourceVisualizer.UpdateProjectionMatrix(projectionMatrix);
        scene.UpdateProjection(projectionMatrix);


        // Update lights
        phongLightManager.SetSpotlightPosition(camera.GetCameraPosition());
        phongLightManager.SetSpotlightDirection(camera.GetCameraDirection());
        if (flashLightOn)
            phongLightManager.SetSpotlightColor(Vec3f(1.f, 1.f, 1.f));
        else
            phongLightManager.SetSpotlightColor(Vec3f(0.f, 0.f, 0.f));
        phongLightManager.UpdateLights();
        lightSourceVisualizer.UpdateCamera(camera.GetWorldToCamMatrix());
        for (U32 i = 0; i < phongLightManager.GetNumPointLights(); ++i)
            lightSourceVisualizer.RenderPointLight(phongLightManager.GetPointLightPosition(i),
                                                   phongLightManager.GetPointLightDiffuse(i));

        scene.Render();

        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
