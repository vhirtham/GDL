#include "gdl/base/timer.h"
#include "gdl/input/inputState.h"
#include "gdl/input/key.h"
#include "gdl/math/vec3.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"


#include "applications/examples/opengl/utility/defaultRenderSetup.h"
#include "applications/examples/opengl/utility/exampleSceneGenerator.h"
#include "applications/examples/opengl/utility/exampleCamera.h"
#include "applications/examples/opengl/utility/exampleLightSourceManager.h"


using namespace GDL;
using namespace GDL::Input;
using namespace GDL::OpenGL;
using namespace GDL::TransformationMatrix4;



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

            struct PointLight
            {
                vec3 position;
                vec3 color;

                float ambientStrength;
                float specularStrength;
                float specularPower;

                float attenuationConstant;
                float attenuationLinear;
                float attenuationQuadratic;
            };

            struct Material
            {
                sampler2D albedo;
                sampler2D specular;
                float reflectivity;
            };


            uniform vec4 cameraPosition;
            uniform PointLight pointLight;
            uniform Material material;

            in vec3 positionWorld;
            in vec3 normal;
            in vec2 texCoord;

            out vec4 fragColor;


            void main(void)
            {
                // Albedo and specular
                // -------------------
                vec4 albedo = texture(material.albedo, texCoord);
                vec4 materialSpecular = texture(material.specular, texCoord);

                // Attenuation
                // -----------
                float distance = length(pointLight.position - positionWorld);
                float attenuation = 1.f / (pointLight.attenuationConstant +
                                           pointLight.attenuationLinear * distance +
                                           pointLight.attenuationQuadratic * distance * distance);

                // Ambient light component
                // -----------------------
                vec3 ambient = albedo.xyz * pointLight.color * 0.1;


                // Diffuse light component
                // -----------------------
                vec3 unitNormal = normalize(normal);
                vec3 lightDirection = normalize(pointLight.position - positionWorld);
                float diffuseStrength = max(dot(unitNormal, lightDirection), 0.f);
                vec3 diffuse = albedo.xyz * pointLight.color * diffuseStrength;

                // Specular light component
                // ------------------------
                vec3 cameraDirection = normalize(cameraPosition.xyz - positionWorld);
                vec3 reflectionDirection = reflect(-lightDirection, unitNormal);
                float spec = pow(max(dot(cameraDirection, reflectionDirection), 0.f), material.reflectivity);
                vec3 specular = materialSpecular.xyz * pointLight.color * spec;



                fragColor = vec4(attenuation * (ambient + diffuse + specular), albedo.w);
            }
            )glsl";
}



// Main ---------------------------------------------------------------------------------------------------------------

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context ----------------------------------------------

    auto [renderWindow, contextManager] = DefaultRenderSetup();
    renderWindow.SetTitle("movement: W,A,S,D - capture mouse: C - release mouse: ESC");



    // Create Shader and Program -----------------------------------------

    Shader vertexShader(GL_VERTEX_SHADER, GetVertexShaderCode());
    Shader fragmentShader(GL_FRAGMENT_SHADER, GetFragmentShaderCode());
    Program program(vertexShader, fragmentShader);



    // Set uniform sampler texture units ---------------------------------

    program.SetUniformSamplerTextureUnit("material.albedo", 0);
    program.SetUniformSamplerTextureUnit("material.specular", 1);



    // Create default scene ----------------------------------------------

    ExampleSceneGenerator scene(program);
    scene.CreateExampleScene01();



    // Light source ------------------------------------------------------

    Vec3f lightPosition(0, 3, 3);
    Vec3f lightColor(1, 1, 1);
    F32 attenuationConstant = 1;
    F32 attenuationLinear = 0.09f;
    F32 attenuationQuadratic = 0.032f;

    program.SetUniform(program.QueryUniformLocation("pointLight.position"), lightPosition);
    program.SetUniform(program.QueryUniformLocation("pointLight.color"), lightColor);
    program.SetUniform(program.QueryUniformLocation("pointLight.attenuationConstant"), attenuationConstant);
    program.SetUniform(program.QueryUniformLocation("pointLight.attenuationLinear"), attenuationLinear);
    program.SetUniform(program.QueryUniformLocation("pointLight.attenuationQuadratic"), attenuationQuadratic);



    // Camera ------------------------------------------------------------

    ExampleCamera camera(0, -5, 1.5);
    camera.SetMovementSpeed(2);
    GLint worldCameraLocation = program.QueryUniformLocation("worldCamera");



    // Create light source manager ---------------------------------------

    ExampleLightSourceManager lightSourceManager;



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


        camera.MoveCamera(deltaTime);
        program.SetUniform(program.QueryUniformLocation("cameraPosition"), camera.GetCameraPosition());
        program.SetUniform(worldCameraLocation, camera.GetWorldToCamMatrix());

        // Update projection
        Mat4f projectionMatrix = PerspectiveProjection(45.f, static_cast<F32>(renderWindow.GetWidth()),
                                                       static_cast<F32>(renderWindow.GetHeight()), 0.1f, 100.f);
        lightSourceManager.UpdateProjectionMatrix(projectionMatrix);
        scene.UpdateProjection(projectionMatrix);

        lightSourceManager.UpdateCamera(camera.GetWorldToCamMatrix());
        lightSourceManager.RenderPointLight(lightPosition, lightColor);

        scene.Render();

        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
