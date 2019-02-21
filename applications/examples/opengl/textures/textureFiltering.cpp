#include "gdl/base/container/vector.h"
#include "gdl/base/timer.h"
#include "gdl/math/transformations4.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/contextManager.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/renderWindow.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/texture.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"
#include "gdl/rendering/textures/textureData2d.h"

#include "applications/examples/opengl/utility/meshGenerator.h"



using namespace GDL;
using namespace GDL::OpenGL;



int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context ----------------------------------------------

    ContextManager& contextManager = ContextManager::Instance();
    contextManager.EnableDebug();
    RenderWindow renderWindow(contextManager);
    renderWindow.SetTitle("Indexed Drawing");
    renderWindow.Initialize();



    // Shader Code -------------------------------------------------------

    const char* vertexShaderCode = R"glsl(
            #version 430

            layout (location=0) in vec3 vertexPosition;
            layout (location=1) in vec2 vertexTexCoord;

            out vec2 texCoord;

            uniform mat4 projection;
            uniform mat4 modelWorld;

            uniform float factor;

            void main(void)
            {
                texCoord = vertexTexCoord * factor;
                gl_Position = projection * modelWorld * vec4(vertexPosition,1.0);
            }
            )glsl";

    const char* fragmentShaderCode = R"glsl(
            #version 430

            in vec2 texCoord;
            out vec4 fragColor;

            uniform sampler2D texture0;

            void main(void)
            {
                fragColor = texture(texture0,texCoord);
            }
            )glsl";



    // Create Shader and Program -----------------------------------------

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    Program program(vertexShader, fragmentShader);



    // Create Buffer Objects ---------------------------------------------
    auto [vertexData, elementData] = MeshGenerator::CreateRectangle<false, true>();

    BufferObject vertexBuffer(vertexData, GL_STATIC_DRAW);
    BufferObject elementBuffer(elementData, GL_STATIC_DRAW);



    // Create Texture ----------------------------------------------------

    TextureData2d textureData = TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/testImage.tex").c_str());
    Texture texture(textureData);



    // Create Vertex Array Object ----------------------------------------

    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, vertexBuffer, GL_FLOAT, 3, 20);
    vao.EnableAttribute(1, 0, GL_FLOAT, 2, 12);
    vao.SetElementBuffer(elementBuffer);


    // Uniforms ----------------------------------------------------------

    GLint uniformProjection = program.QueryUniformLocation("projection");
    GLint uniformModelWorld = program.QueryUniformLocation("modelWorld");
    GLint uniformFactor = program.QueryUniformLocation("factor");



    // Set state ---------------------------------------------------------

    vao.Bind();
    texture.Bind(0);
    program.Use();

    // Other -------------------------------------------------------------

    Timer timer;
    F32 factor = 1.f;
    bool increasing = false;


    // Start main loop ---------------------------------------------------

    while (renderWindow.IsOpen())
    {
        F32 deltaFactor = timer.GetElapsedTime<Milliseconds>().count() / 1000.f / ((factor < 1.f) ? 10.f : 10.f) *
                          ((increasing) ? 1.f : -1.f);
        timer.Reset();

        factor += deltaFactor;
        if (increasing)
        {
            if (factor >= 3.f)
                increasing = false;
        }
        else if (factor <= 0.2f)
            increasing = true;



        program.SetUniform(uniformFactor, factor);

        F32 width = textureData.GetWidth() * 2;
        F32 height = textureData.GetHeight() * 2;
        F32 translateX = width * 0.55f;


        contextManager.PollEvents();

        program.SetUniform(uniformProjection, Transformations4::OrthogonalProjection(renderWindow.GetWidth(),
                                                                                          renderWindow.GetHeight()));

        // Right texture ---> Nearest
        texture.SetMinifyingFilter(GL_NEAREST);
        texture.SetMagnifyingFilter(GL_NEAREST);
        program.SetUniform(uniformModelWorld,
                           Transformations4::ScaleTranslate(width, height, 1, -translateX, 0, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        // Left texture ---> Linear
        texture.SetMinifyingFilter(GL_LINEAR);
        texture.SetMagnifyingFilter(GL_LINEAR);
        program.SetUniform(uniformModelWorld,
                           Transformations4::ScaleTranslate(width, height, 1, translateX, 0, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
