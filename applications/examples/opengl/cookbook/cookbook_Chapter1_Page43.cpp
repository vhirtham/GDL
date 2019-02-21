// GDL version of the sample code from
// OpenGL 4 Shading Language Cookbook - Second Edtion - Chapter 1 - Page 43:
#define TITLE "Using uniform blocks and uniform buffer objects"



#include "gdl/base/time.h"
#include "gdl/base/timer.h"
#include "gdl/base/container/vector.h"
#include "gdl/math/transformations4.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/contextManager.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/renderWindow.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

using namespace GDL;
using namespace GDL::OpenGL;
using namespace GDL::Transformations4;



// Support classes --------------------------------------------------------------------------------

//! @brief This an example of how one can implement a uniform buffor object management class for a specific uniform
//! block. Since the requirements are highly dependent on the use case and structure of the uniform block, a generalized
//! management class would have to cover too much features that might not be used most of the time or come with
//! performance penalties.
//! The general idea of this class structure is to have a an array which contains all the offsets of the uniforms of a
//! uniform block. To avoid indexing issues when adding or removing uniforms from the block, we introduce an enum with
//! a value for each uniform. Each access to the offset array is done by using the enum instead of a plain number.
//! Setters for all uniforms are added to write the data at the corresponding offset into a large buffer. When a
//! submit function is called, the current buffer is uploaded to the buffer object.
class TransformationsUBO
{
    static constexpr U32 numUniforms = 2;

    //! @brief Helper enum. The values must be in the range [0, numUniforms]
    enum
    {
        PROJECTION_MATRIX = 0,
        MODELWORLD_MATRIX
    };



    Vector<U8> mData;
    BufferObject mUBO;
    std::array<GLint, numUniforms> mUniformOffsets;

public:
    TransformationsUBO(const Program& program, GLuint uniformBlockIndex)
        : mData(static_cast<U32>(program.QueryUniformBlockSize(uniformBlockIndex)))
        , mUBO(static_cast<GLint>(mData.size()), GL_DYNAMIC_DRAW)
        , mUniformOffsets{program.QueryUniformBlockUniformOffsets<numUniforms>(CreateUniformNameArray())}
    {
        mUBO.BindAsUniformBuffer(program.QueryUniformBlockBinding(uniformBlockIndex));
    }

    void SetProjectionMatrix(const Mat4f& matrix)
    {
        SetMatrix(PROJECTION_MATRIX, matrix);
    }

    void SetModelWorldMatrix(const Mat4f& matrix)
    {
        SetMatrix(MODELWORLD_MATRIX, matrix);
    }

    void Submit()
    {
        mUBO.SetData(mData);
    }

private:
    void SetMatrix(U32 uniformIndex, const Mat4f& matrix)
    {
        static_assert(sizeof(Mat4f) == 16 * sizeof(F32), "Matrix size is not as expected");
        std::memcpy(mData.data() + mUniformOffsets[uniformIndex], &matrix, sizeof(Mat4f));
    }


    const char* GetUniformName(U32 uniformIndex) const
    {
        switch (uniformIndex)
        {
        case PROJECTION_MATRIX:
            return "ProjectionMatrix";
        case MODELWORLD_MATRIX:
            return "ModelWorldMatrix";
        default:
            THROW("Invalid value");
        }
    }

    //! @brief Creates an array with uniform names. All entries are at the position corresponding to the enum value. If
    //! there is no enum value for an index in the range of [0,numUniforms], this function will throw. It should only be
    //! used during initialization
    std::array<const char*, numUniforms> CreateUniformNameArray()
    {
        std::array<const char*, numUniforms> uniformNames;
        for (U32 i = 0; i < uniformNames.size(); ++i)
            uniformNames[i] = GetUniformName(i);
        return uniformNames;
    }
};


// Main -------------------------------------------------------------------------------------------

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context ---------------------

    ContextManager& contextManager = ContextManager::Instance();
    contextManager.EnableDebug();
    RenderWindow renderWindow(contextManager);
    renderWindow.SetTitle(TITLE);
    renderWindow.Initialize();



    // Shader Code ------------------------------

    const char* vertexShaderCode = R"glsl(
            #version 450

            layout (location=0) in vec3 VertexPosition;
            layout (location=1) in vec2 vertexTexCoordinate;

            out vec2 texCoordinate;

            layout (std140) uniform Transformations
            {
                mat4 ProjectionMatrix;
                mat4 ModelWorldMatrix;
            };

            void main(void)
            {
                texCoordinate = vertexTexCoordinate;
                gl_Position = ProjectionMatrix * ModelWorldMatrix * vec4(VertexPosition,1.0);            
            }
            )glsl";

    const char* fragmentShaderCircleCode = R"glsl(
            #version 450

            in vec2 texCoordinate;

            out vec4 FragColor;
            void main(void)
            {
                float intensity = 1-sqrt((texCoordinate.x * texCoordinate.x + texCoordinate.y * texCoordinate.y));
                FragColor = vec4(vec3(1.0,0.2,1.0) * intensity,1.0);
            }
            )glsl";

    const char* fragmentShaderSquareCode = R"glsl(
            #version 450

            in vec2 texCoordinate;

            out vec4 FragColor;
            void main(void)
            {
                FragColor = vec4(vec3(0.0,0.2,1.0),1.0);
            }
            )glsl";



    // Create Shader and Program ----------------

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShaderCircle(GL_FRAGMENT_SHADER, fragmentShaderCircleCode);
    Shader fragmentShaderSquare(GL_FRAGMENT_SHADER, fragmentShaderSquareCode);

    Program programCircle(vertexShader, fragmentShaderCircle);
    Program programSquare(vertexShader, fragmentShaderSquare);


    constexpr U32 bindingPoint = 2;
    programCircle.SetUniformBlockBinding(programCircle.QueryUniformBlockIndex("Transformations"), bindingPoint);
    programSquare.SetUniformBlockBinding(programSquare.QueryUniformBlockIndex("Transformations"), bindingPoint);

    // Create Buffer Objects --------------------
    // clang-format off
    Vector<F32> positionData
    {
        -1.0f, -1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         1.0f,  1.0f,  0.0f
    };

    Vector<F32> texCoordData
    {
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
    };
    // clang-format on

    BufferObject positionBuffer(positionData, GL_STATIC_DRAW);
    BufferObject texCoordBuffer(texCoordData, GL_STATIC_DRAW);


    // Create Vertex Array Object ---------------
    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, positionBuffer, GL_FLOAT, 3, 12);
    vao.EnableAttribute(1, 1, texCoordBuffer, GL_FLOAT, 2, 8);



    // Bind vertex array object -----------------
    vao.Bind();

    // Uniform buffer object --------------------

    TransformationsUBO transformationsUBO(programCircle, programCircle.QueryUniformBlockIndex("Transformations"));

    // Constant rotation setup ------------------

    TimePoint startTime = CurrentTime();
    TimePoint prevTime = CurrentTime();
    TimePoint currTime = CurrentTime();

    F32 angle = 0.;
    F32 translateX = 0.;
    F32 translateY = 0.;

    while (renderWindow.IsOpen())
    {
        contextManager.PollEvents();
        prevTime = currTime;
        currTime = CurrentTime();

        F32 frameTimeMS = std::chrono::duration_cast<Microseconds>(currTime - prevTime).count();
        F32 elapsedTimeMS = std::chrono::duration_cast<Microseconds>(currTime - startTime).count();

        angle += 3.14f * frameTimeMS / 5.e5f;
        translateX = std::sin(elapsedTimeMS / 3.e6f) * renderWindow.GetWidth() / 3.f;
        translateY = std::sin(elapsedTimeMS / 4.e6f) * renderWindow.GetHeight() / 3.f;

        Mat4f projectionMatrix = OrthogonalProjection(renderWindow.GetWidth(), renderWindow.GetHeight());
        Mat4f modelWorldMatrix = Translation(translateX, translateY, 0.) * RotationZ(angle) * Scale(200.f, 200.f, 1);

        transformationsUBO.SetProjectionMatrix(projectionMatrix);
        transformationsUBO.SetModelWorldMatrix(modelWorldMatrix);
        transformationsUBO.Submit();

        if (translateX > 0)
            programCircle.Use();
        else
            programSquare.Use();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        renderWindow.SwapBuffers();
    }
}
