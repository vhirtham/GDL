#include "gdl/base/container/vector.h"
#include "gdl/base/timer.h"
#include "gdl/math/transformationMatrix.h"
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

           uniform mat4 projection;
           uniform mat4 modelWorld;

           void main(void)
           {
               texCoord = vertexTexCoord;
               gl_Position = projection * modelWorld * vec4(vertexPosition,1.0);
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
               fragColor = texture(texture0,texCoord);
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
    renderWindow.SetTitle("Indexed Drawing");
    renderWindow.Initialize();



    // Create Shader and Program -----------------------------------------

    Shader vertexShader(GL_VERTEX_SHADER, GetVertexShaderCode());
    Shader fragmentShader(GL_FRAGMENT_SHADER, GetFragmentShaderCode());
    Program program(vertexShader, fragmentShader);



    // Create Buffer Objects ---------------------------------------------
    auto [vertexData, elementData] = MeshGenerator::CreateRectangle<true>();

    BufferObject vertexBuffer(vertexData, GL_STATIC_DRAW);
    BufferObject elementBuffer(elementData, GL_STATIC_DRAW);



    // Create Texture ----------------------------------------------------

    TextureData2d textureData = TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/testImage.tex").c_str());
    textureData.CreateMipMaps(Interpolation::LINEAR);

    //    constexpr U32 level = 1;
    //    Vector<U8> pixelData(textureData.GetWidth(level) * textureData.GetHeight(level) *
    //    textureData.GetNumChannels()); for (U32 i = 0; i < pixelData.size(); ++i)
    //        switch (i % 4)
    //        {
    //        case 0:
    //        case 3:
    //            pixelData[i] = 255;
    //            break;
    //        default:
    //            pixelData[i] = 0;
    //        }
    //    textureData.SetMipMapData(level, pixelData);

    Texture texture(textureData);
    //    texture.CreateMipMaps();



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



    // Other -------------------------------------------------------------

    Timer timer;
    F32 scale = 1.f;
    bool increasing = false;

    F32 translateX = textureData.GetWidth() * 0.55f;
    F32 translateY = textureData.GetHeight() * 0.55f;



    // Start main loop ---------------------------------------------------

    while (renderWindow.IsOpen())
    {
        F32 deltaFactor = timer.GetElapsedTime<Milliseconds>().count() / 1000.f / ((scale < 1.f) ? 10.f : 10.f) *
                          ((increasing) ? 1.f : -1.f);
        timer.Reset();

        scale += deltaFactor;
        if (increasing)
        {
            if (scale >= 1.f)
                increasing = false;
        }
        else if (scale <= 0.1f)
            increasing = true;


        F32 width = textureData.GetWidth() * scale;
        F32 height = textureData.GetHeight() * scale;

        contextManager.PollEvents();

        program.SetUniform(uniformProjection, TransformationMatrix4::OrthogonalProjection(renderWindow.GetWidth(),
                                                                                          renderWindow.GetHeight()));

        // Lower left texture ---> Linerar - linear
        texture.SetMinifyingFilter(GL_LINEAR_MIPMAP_LINEAR);
        texture.SetMagnifyingFilter(GL_LINEAR);
        program.SetUniform(uniformModelWorld,
                           TransformationMatrix4::ScaleTranslate(width, height, 1, -translateX, -translateY, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Upper left texture ---> Linear - nearest
        texture.SetMinifyingFilter(GL_LINEAR_MIPMAP_NEAREST);
        texture.SetMagnifyingFilter(GL_LINEAR);
        program.SetUniform(uniformModelWorld,
                           TransformationMatrix4::ScaleTranslate(width, height, 1, -translateX, translateY, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Upper right texture ---> Nearest - linear
        texture.SetMinifyingFilter(GL_NEAREST_MIPMAP_LINEAR);
        texture.SetMagnifyingFilter(GL_NEAREST);
        program.SetUniform(uniformModelWorld,
                           TransformationMatrix4::ScaleTranslate(width, height, 1, translateX, translateY, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Lower right texture ---> Nearest - nearest
        texture.SetMinifyingFilter(GL_NEAREST_MIPMAP_NEAREST);
        texture.SetMagnifyingFilter(GL_NEAREST);
        program.SetUniform(uniformModelWorld,
                           TransformationMatrix4::ScaleTranslate(width, height, 1, translateX, -translateY, 0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        renderWindow.SwapBuffers();
    }

    exit(EXIT_SUCCESS);
}
