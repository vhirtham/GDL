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
#include "applications/examples/opengl/utility/exampleSceneGenerator.h"
#include "applications/examples/opengl/utility/meshGenerator.h"


using namespace GDL;
using namespace GDL::Input;
using namespace GDL::OpenGL;
using namespace GDL::TransformationMatrix4;



// ShaderCode ---------------------------------------------------------------------------------------------------------

const char* GetVertexShaderCode()
{
    return R"glsl(
            #version 430

            layout (location=0) in vec3 vertexPosition;
            layout (location=1) in vec3 vertexNormal;
            layout (location=2) in vec2 vertexTexCoord;

            uniform mat4 projection;
            uniform mat4 modelWorld;
            uniform mat4 worldCamera;
            uniform vec2 textureScale;
            uniform vec2 textureOffset;

            out vec2 texCoord;

            void main(void)
            {
                texCoord = (vertexTexCoord + textureOffset) * textureScale;
                gl_Position =  projection * worldCamera * modelWorld * vec4(vertexPosition, 1);
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


    // Set state ---------------------------------------------------------



    // Misc --------------------------------------------------------------


    GLint worldCameraLocation = program.QueryUniformLocation("worldCamera");

    F32 camX = 0;
    F32 camY = 0;
    F32 camZ = 1;

    F32 prevMouseX = 0;
    F32 prevMouseY = 0;

    F32 mouseSensitivity = 1.f / 100.f;

    F32 yaw = 0;
    F32 pitch = 0;

    // Start main loop ---------------------------------------------------

    Timer timer;
    while (renderWindow.IsOpen())
    {
        F32 deltaTime = timer.GetElapsedTime<Microseconds>().count() / 1000000.f;
        timer.Reset();

        contextManager.PollEvents();

        // Mouse inputs -------------------------
        F32 mouseDeltaX = static_cast<F32>(InputState::GetMousePositionX()) - prevMouseX;
        F32 mouseDeltaY = static_cast<F32>(InputState::GetMousePositionY()) - prevMouseY;
        prevMouseX = static_cast<F32>(InputState::GetMousePositionX());
        prevMouseY = static_cast<F32>(InputState::GetMousePositionY());

        yaw += (mouseDeltaX * mouseSensitivity);
        pitch += (mouseDeltaY * mouseSensitivity);
        if (pitch > static_cast<F32>(M_PI) / 2.f)
            pitch = static_cast<F32>(M_PI) / 2.f;
        if (pitch < static_cast<F32>(-M_PI) / 2.f)
            pitch = static_cast<F32>(-M_PI) / 2.f;


        // Keyboard inputs ----------------------
        F32 moveCamX = 0;
        F32 moveCamY = 0;

        if (InputState::GetKeyPressed(Key::W))
            moveCamY += 1;
        if (InputState::GetKeyPressed(Key::S))
            moveCamY -= 1;
        if (InputState::GetKeyPressed(Key::D))
            moveCamX += 1;
        if (InputState::GetKeyPressed(Key::A))
            moveCamX -= 1;
        if (InputState::GetKeyPressed(Key::ESC))
            renderWindow.ReleaseCursor();
        if (InputState::GetKeyPressed(Key::C))
            renderWindow.CaptureCursor();


        // Calculate movement -------------------
        F32 sinYaw = std::sin(yaw);
        F32 cosYaw = std::cos(yaw);
        F32 moveLength = std::sqrt(moveCamX * moveCamX + moveCamY * moveCamY);
        F32 normalizeFactor = (moveLength != ApproxZero<F32>()) ? 1.f / moveLength : 1.f;
        camX += (moveCamY * sinYaw + moveCamX * cosYaw) * normalizeFactor * deltaTime;
        camY += (moveCamY * cosYaw + moveCamX * -sinYaw) * normalizeFactor * deltaTime;


        // Update worldCam matrix ---------------
        program.SetUniform(worldCameraLocation, RotationX(pitch) * RotationY(yaw) *
                                                        RotationX(static_cast<F32>(-M_PI) / 2.f) *
                                                        Translation(-camX, -camY, -camZ));


        scene.UpdateProjection(
                PerspectiveProjection(45.f, renderWindow.GetWidth(), renderWindow.GetHeight(), 0.1f, 100.f));
        scene.Render();

        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
