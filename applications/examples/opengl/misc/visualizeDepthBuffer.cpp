#include "gdl/base/timer.h"
#include "gdl/input/inputState.h"
#include "gdl/input/key.h"
#include "gdl/rendering/glsl/linearizeDepth.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"


#include "applications/examples/opengl/utility/defaultRenderSetup.h"
#include "applications/examples/opengl/utility/exampleSceneGenerator.h"
#include "applications/examples/opengl/utility/exampleCamera.h"


using namespace GDL;
using namespace GDL::GLSL;
using namespace GDL::Input;
using namespace GDL::OpenGL;
using namespace GDL::Transformations4;



// ShaderCode ---------------------------------------------------------------------------------------------------------



const char* GetVertexShaderCode()
{
    return R"glsl(
            #version 450

            layout (location=0) in vec3 vertexPosition;


            uniform mat4 projection;
            uniform mat4 modelWorld;
            uniform mat4 worldCamera;

            void main(void)
            {
                gl_Position =  projection * worldCamera * modelWorld * vec4(vertexPosition, 1.f);
            }
            )glsl";
}



const char* GetFragmentShaderCode()
{
    return R"glsl(
            #version 450

            float LinearizeDepth(float);

            out vec4 fragColor;

            void main(void)
            {
                fragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z)), 1.f);
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
    Shader linearizeDepthShader(GL_FRAGMENT_SHADER, GetLinearizeDepthShaderCode());
    Shader fragmentShader(GL_FRAGMENT_SHADER, GetFragmentShaderCode());
    Program program(vertexShader, linearizeDepthShader, fragmentShader);



    // Create default scene ----------------------------------------------

    ExampleSceneGenerator scene(program);
    scene.CreateExampleScene01();



    // Camera ------------------------------------------------------------

    ExampleCamera camera(0, -5, 1.5);
    camera.SetMovementSpeed(2);
    GLint worldCameraLocation = program.QueryUniformLocation("worldCamera");



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


        // Camera movement
        camera.MoveCamera(deltaTime);
        program.SetUniform(worldCameraLocation, camera.GetWorldToCamMatrix());


        // Update projection
        Mat4f projectionMatrix = PerspectiveProjection(45.f, static_cast<F32>(renderWindow.GetWidth()),
                                                       static_cast<F32>(renderWindow.GetHeight()), 0.1f, 100.f);
        scene.UpdateProjection(projectionMatrix);

        scene.Render();

        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
