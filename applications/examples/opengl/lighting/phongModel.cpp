#include "gdl/base/timer.h"
#include "gdl/input/inputState.h"
#include "gdl/input/key.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"


#include "applications/examples/opengl/utility/defaultRenderSetup.h"
#include "applications/examples/opengl/utility/exampleSceneGenerator.h"
#include "applications/examples/opengl/utility/exampleCamera.h"



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
            uniform vec2 textureScale;
            uniform vec2 textureOffset;

            out vec2 texCoord;

            void main(void)
            {
                texCoord = (vertexTexCoord + textureOffset) * textureScale;
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
    renderWindow.SetTitle("movement: W,A,S,D - capture mouse: C - release mouse: ESC");



    // Create Shader and Program -----------------------------------------

    Shader vertexShader(GL_VERTEX_SHADER, GetVertexShaderCode());
    Shader fragmentShader(GL_FRAGMENT_SHADER, GetFragmentShaderCode());
    Program program(vertexShader, fragmentShader);



    // Set uniform sampler texture units ---------------------------------

    program.SetUniformSamplerTextureUnit("texture0", 0);



    // Create default scene ----------------------------------------------

    ExampleSceneGenerator scene(program);
    scene.CreateExampleScene01();



    // Camera ------------------------------------------------------------

    ExampleCamera camera(0, 0, 1);
    GLint worldCameraLocation = program.QueryUniformLocation("worldCameraMatrix");



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
        program.SetUniform(worldCameraLocation, camera.GetWorldToCamMatrix());


        scene.UpdateProjection(renderWindow.GetWidth(), renderWindow.GetHeight());
        scene.Render();

        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
