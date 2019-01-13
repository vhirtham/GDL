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
    renderWindow.SetTitle("movement: W,A,S,D - capture mouse: C - release mouse: ESC");


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
        F32 moveCamZ = 0;
        F32 moveCamX = 0;

        if (InputState::GetKeyPressed(Key::W))
            moveCamZ += 1;
        if (InputState::GetKeyPressed(Key::S))
            moveCamZ -= 1;
        if (InputState::GetKeyPressed(Key::A))
            moveCamX += 1;
        if (InputState::GetKeyPressed(Key::D))
            moveCamX -= 1;
        if (InputState::GetKeyPressed(Key::ESC))
            renderWindow.ReleaseCursor();
        if (InputState::GetKeyPressed(Key::C))
            renderWindow.CaptureCursor();


        // Calculate movement -------------------
        F32 sinYaw = std::sin(yaw);
        F32 cosYaw = std::cos(yaw);
        F32 moveLength = std::sqrt(moveCamX * moveCamX + moveCamZ * moveCamZ);
        F32 normalizeFactor = (moveLength != ApproxZero<F32>()) ? 1.f / moveLength : 1.f;
        camZ += (moveCamZ * cosYaw + moveCamX * sinYaw) * normalizeFactor * deltaTime;
        camX += (moveCamZ * -sinYaw + moveCamX * cosYaw) * normalizeFactor * deltaTime;


        // Update worldCam matrix ---------------
        program.SetUniform(worldCameraLocation, TransformationMatrix4::RotationX(pitch) *
                                                        TransformationMatrix4::RotationY(yaw) *
                                                        TransformationMatrix4::Translation(camX, camY, camZ));


        scene.UpdateProjection(renderWindow.GetWidth(), renderWindow.GetHeight());
        scene.Render();
        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}