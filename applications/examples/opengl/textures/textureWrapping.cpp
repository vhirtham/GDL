#include "gdl/base/container/vector.h"
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

            void main(void)
            {
                texCoord = vertexTexCoord * 3 - 1;
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

    glTextureParameteri(texture.GetHandle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create Vertex Array Object ----------------------------------------

    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, vertexBuffer, GL_FLOAT, 3, 20);
    vao.EnableAttribute(1, 0, GL_FLOAT, 2, 12);
    vao.SetElementBuffer(elementBuffer);


    // Uniforms ----------------------------------------------------------

    GLint uniformProjection = program.QueryUniformLocation("projection");
    GLint uniformModelWorld = program.QueryUniformLocation("modelWorld");



    // Set state ---------------------------------------------------------

    vao.Bind();
    texture.Bind(0);
    program.Use();



    // Start main loop ---------------------------------------------------

    while (renderWindow.IsOpen())
    {
        contextManager.PollEvents();

        F32 width = renderWindow.GetWidth() / 2.2f;
        F32 height = renderWindow.GetHeight() / 2.2f;
        F32 translateX = width * 0.55f;
        F32 translateY = height * 0.55f;

        program.SetUniform(uniformProjection, Transformations4::OrthogonalProjection(renderWindow.GetWidth(),
                                                                                          renderWindow.GetHeight()));

        // Lower left texture ---> repeat
        texture.SetWrappingXY(GL_REPEAT);
        program.SetUniform(uniformModelWorld,
                           Transformations4::ScaleTranslate(width, height, 1, -translateX, -translateY, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        // Upper left texture ---> mirrored repeat
        texture.SetWrappingXY(GL_MIRRORED_REPEAT);
        program.SetUniform(uniformModelWorld,
                           Transformations4::ScaleTranslate(width, height, 1, -translateX, translateY, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        // Upper right texture ---> clamp to edge
        texture.SetWrappingXY(GL_CLAMP_TO_EDGE);
        program.SetUniform(uniformModelWorld,
                           Transformations4::ScaleTranslate(width, height, 1, translateX, translateY, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        // Lower right texture ---> clamp to border
        texture.SetWrappingXY(GL_CLAMP_TO_BORDER);
        texture.SetBorderColor({{0.f, 0.f, 1.f, 1.f}});
        program.SetUniform(uniformModelWorld,
                           Transformations4::ScaleTranslate(width, height, 1, translateX, -translateY, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
