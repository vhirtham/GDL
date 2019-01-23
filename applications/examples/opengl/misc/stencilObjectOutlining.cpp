#include "gdl/base/timer.h"
#include "gdl/base/container/vector.h"
#include "gdl/math/constants.h"
#include "gdl/math/transformationMatrix.h"
#include "gdl/input/inputState.h"
#include "gdl/input/key.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/texture.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"
#include "gdl/rendering/textures/textureData2d.h"

#include "applications/examples/opengl/utility/exampleCamera.h"
#include "applications/examples/opengl/utility/defaultRenderSetup.h"
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

            out vec2 texCoord;

            void main(void)
            {
                texCoord = vertexTexCoord * textureScale;
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


const char* GetOutlineFragmentShaderCode()
{
    return R"glsl(
            #version 430

            out vec4 fragColor;

            void main(void)
            {
                fragColor = vec4(1.f, 0.f, 0.f, 1.f);
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
    Shader outlineFragmentShader(GL_FRAGMENT_SHADER, GetOutlineFragmentShaderCode());
    Program program(vertexShader, fragmentShader);
    Program programOutline(vertexShader, outlineFragmentShader);



    // Create Buffer Objects ---------------------------------------------

    auto [vertexDataCuboid, elementDataCuboid] = MeshGenerator::CreateCuboid<true, true>();
    BufferObject vertexBuffer(vertexDataCuboid, GL_STATIC_DRAW);
    BufferObject elementBuffer(elementDataCuboid, GL_STATIC_DRAW);



    // Create Vertex Array Objects ---------------------------------------

    VertexArrayObject vaoCuboid;
    vaoCuboid.EnableAttribute(0, 0, vertexBuffer, GL_FLOAT, 3, 32);
    vaoCuboid.EnableAttribute(1, 0, GL_FLOAT, 3, 12);
    vaoCuboid.EnableAttribute(2, 0, GL_FLOAT, 2, 24);
    vaoCuboid.SetElementBuffer(elementBuffer);



    // Camera ------------------------------------------------------------

    ExampleCamera camera(0, 0, 0);



    // Get uniform locations ---------------------------------------------

    GLint projectionLocation = program.QueryUniformLocation("projection");
    GLint modelWorldLocation = program.QueryUniformLocation("modelWorld");
    GLint worldCameraLocation = program.QueryUniformLocation("worldCamera");
    GLint textureScaleLocation = program.QueryUniformLocation("textureScale");

    GLint projectionOutlineLocation = programOutline.QueryUniformLocation("projection");
    GLint modelWorldOutlineLocation = programOutline.QueryUniformLocation("modelWorld");
    GLint worldCameraOutlineLocation = programOutline.QueryUniformLocation("worldCamera");


    // Textures ----------------------------------------------------------

    Texture texture1(TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/redBrick02_2k.tex").c_str()));
    program.SetUniformSamplerTextureUnit("texture0", 0);
    renderWindow.EnableStencilTest();


    // Misc --------------------------------------------------------------

    Mat4f modelWorldMatrixCuboid = Translation(1, 5, 0);



    // Start main loop ---------------------------------------------------

    glDisable(GL_CULL_FACE);
    Timer timer;
    while (renderWindow.IsOpen())
    {
        F32 deltaTime = timer.GetElapsedTime<Microseconds>().count() / 1000000.f;
        timer.Reset();

        contextManager.PollEvents();


        // Keyboard inputs ----------------------

        if (InputState::GetKeyPressed(Key::ESC))
            renderWindow.ReleaseCursor();
        if (InputState::GetKeyPressed(Key::C))
            renderWindow.CaptureCursor();


        // Update projection -------------------

        Mat4f projectionMatrix =
                PerspectiveProjection(45.f, renderWindow.GetWidth(), renderWindow.GetHeight(), 0.1f, 100.f);
        program.SetUniform(projectionLocation, projectionMatrix);
        programOutline.SetUniform(projectionOutlineLocation, projectionMatrix);


        // Update worldCam matrix ---------------
        camera.MoveCamera(deltaTime);
        Mat4f worldCamMatrix = camera.GetWorldToCamMatrix();
        program.SetUniform(worldCameraLocation, worldCamMatrix);
        programOutline.SetUniform(worldCameraOutlineLocation, worldCamMatrix);



        // Render cuboid -----------------------

        program.Use();
        vaoCuboid.Bind();
        program.SetUniform(modelWorldLocation, modelWorldMatrixCuboid);
        program.SetUniform(textureScaleLocation, std::array<F32, 2>{1, 1});
        texture1.Bind(0);

        renderWindow.EnableDepthTest();
        renderWindow.SetStencilTestAction(GL_KEEP, GL_KEEP, GL_REPLACE);

        renderWindow.SetStencilFunction(GL_ALWAYS, 1, 0xFF);
        renderWindow.EnableWriteToStencilBuffer();

        glDrawElements(GL_TRIANGLES, static_cast<GLint>(elementDataCuboid.size()), GL_UNSIGNED_INT, nullptr);

        // Render cuboids border ----------------

        programOutline.Use();
        programOutline.SetUniform(modelWorldOutlineLocation, modelWorldMatrixCuboid * Scale(1.02f, 1.02f, 1.02f));

        renderWindow.SetStencilFunction(GL_NOTEQUAL, 1, 0xFF);
        renderWindow.DisableWriteToStencilBuffer();
        renderWindow.DisableDepthTest();

        glDrawElements(GL_TRIANGLES, static_cast<GLint>(elementDataCuboid.size()), GL_UNSIGNED_INT, nullptr);

        // Reenabling is important, otherwise a clear command (hidden in SwapBuffers) would have no effect
        renderWindow.EnableWriteToStencilBuffer();



        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
