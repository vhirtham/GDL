#define USE_GLUT

#include "gdl/base/container/vector.h"
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



// ShaderCode ---------------------------------------------------------------------------------------------------------

const char* GetVertexShaderCode()
{
    return R"glsl(
            #version 430

            layout (location=0) in vec3 vertexPosition;
            layout (location=1) in vec2 vertexTexCoord;

            out vec2 texCoord;

            void main(void)
            {
                texCoord = vertexTexCoord;
                gl_Position = vec4(vertexPosition,1.0);
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
            uniform sampler2D texture1;

            void main(void)
            {
                vec4 color0 = texture(texture0, texCoord);
                vec4 color1 = texture(texture1, texCoord);

                vec2 coords = texCoord * 2 - 1;

                float factor = min(1, sqrt(coords.x * coords.x + coords.y * coords.y));

                fragColor = mix(color0, color1, factor);
            }
            )glsl";
}



// Main ---------------------------------------------------------------------------------------------------------------

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context ----------------------------------------------

    ContextManager& contextManager = ContextManager::Instance();
    contextManager.EnableDebug();
    RenderWindow renderWindow(contextManager);
    renderWindow.Initialize();



    // Create Shader and Program -----------------------------------------

    Shader vertexShader(GL_VERTEX_SHADER, GetVertexShaderCode());
    Shader fragmentShader(GL_FRAGMENT_SHADER, GetFragmentShaderCode());
    Program program(vertexShader, fragmentShader);



    // Set uniform sampler texture units ---------------------------------

    program.SetUniformSamplerTextureUnit("texture0", 0);
    program.SetUniformSamplerTextureUnit("texture1", 1);



    // Create Buffer Objects ---------------------------------------------
    auto [vertexData, elementData] = MeshGenerator::CreateRectangle<true>();

    BufferObject vertexBuffer(vertexData, GL_STATIC_DRAW);
    BufferObject elementBuffer(elementData, GL_STATIC_DRAW);



    // Create Textures ---------------------------------------------------

    TextureData2d textureData0 = TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/testImage.tex").c_str());
    TextureData2d textureData1 = TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/brickWall.tex").c_str());
    Texture texture0(textureData0);
    Texture texture1(textureData1);



    // Create Vertex Array Object ----------------------------------------

    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, vertexBuffer, GL_FLOAT, 3, 20);
    vao.EnableAttribute(1, 0, GL_FLOAT, 2, 12);
    vao.SetElementBuffer(elementBuffer);



    // Set state ---------------------------------------------------------

    vao.Bind();
    texture0.Bind(0);
    texture1.Bind(1);
    program.Use();



    // Start main loop ---------------------------------------------------

    while (renderWindow.IsOpen())
    {
        contextManager.PollEvents();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
