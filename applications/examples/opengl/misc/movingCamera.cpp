#include "gdl/base/timer.h"
#include "gdl/base/container/vector.h"
#include "gdl/input/inputState.h"
#include "gdl/input/key.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/texture.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"
#include "gdl/rendering/textures/textureData2d.h"

#include "applications/examples/opengl/utility/defaultRenderSetup.h"
#include "applications/examples/opengl/utility/defaultScene01.h"
#include "applications/examples/opengl/utility/meshGenerator.h"


using namespace GDL;
using namespace GDL::Input;
using namespace GDL::OpenGL;



// ShaderCode ---------------------------------------------------------------------------------------------------------

const char* GetVertexShaderCode()
{
    return R"glsl(
            #version 430

            layout (location=0) in vec3 vertexPosition;
            layout (location=1) in vec3 vertexNormal;
            layout (location=2) in vec2 vertexTexCoord;

            uniform mat4 projectionMatrix;
            uniform mat4 modelWorldMatrix;
            uniform mat4 worldCameraMatrix;
            uniform float textureScaleX;
            uniform float textureScaleY;

            out vec2 texCoord;

            void main(void)
            {
                texCoord = vertexTexCoord * vec2(textureScaleX, textureScaleY);
                gl_Position =  projectionMatrix * worldCameraMatrix * modelWorldMatrix * vec4(vertexPosition, 1);
            }
            )glsl";
}



const char* GetFragmentShaderCode()
{
    return R"glsl(
            #version 430

            in vec2 texCoord;
            out vec4 fragColor;

            uniform sampler2D texture0;

            void main(void)
            {
                vec4 color0 = texture(texture0, texCoord);

                fragColor = color0;
            }
            )glsl";
}



// Main ---------------------------------------------------------------------------------------------------------------

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context ----------------------------------------------

    auto [renderWindow, contextManager] = DefaultRenderSetup();


    // Create Shader and Program -----------------------------------------

    Shader vertexShader(GL_VERTEX_SHADER, GetVertexShaderCode());
    Shader fragmentShader(GL_FRAGMENT_SHADER, GetFragmentShaderCode());
    Program program(vertexShader, fragmentShader);



    // Set uniform sampler texture units ---------------------------------

    program.SetUniformSamplerTextureUnit("texture0", 0);



    // Create default scene ----------------------------------------------

    DefaultScene01 scene(program);
    scene.CreateDefaultScene01();


    // Set state ---------------------------------------------------------

    program.Use();


    // Misc --------------------------------------------------------------


    GLint worldCameraLocation = program.QueryUniformLocation("worldCameraMatrix");

    F32 camX = 0;
    F32 camY = 0;
    F32 camZ = 0;


    // Start main loop ---------------------------------------------------

    Timer timer;
    while (renderWindow.IsOpen())
    {
        F32 deltaTime = timer.GetElapsedTime<Microseconds>().count() / 1000000.f;
        timer.Reset();

        contextManager.PollEvents();
        if (InputState::GetKeyPressed(Key::W))
            camZ += deltaTime;
        if (InputState::GetKeyPressed(Key::S))
            camZ -= deltaTime;
        if (InputState::GetKeyPressed(Key::A))
            camX += deltaTime;
        if (InputState::GetKeyPressed(Key::D))
            camX -= deltaTime;



        program.SetUniform(worldCameraLocation, TransformationMatrix4::Translation(camX, camY, camZ));


        scene.UpdateProjection(renderWindow.GetWidth(), renderWindow.GetHeight());
        scene.Render();
        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
