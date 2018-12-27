#include "gdl/base/container/vector.h"
#include "gdl/math/transformationMatrix.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/contextManager.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/renderWindow.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"


#include <cmath>

using namespace GDL;
using namespace GDL::OpenGL;

Vector<F32> CreateTorusMesh(F32 radiusMajor, F32 radiusMinor, U32 numMajorSegments, U32 numMinorSegments)
{
    Vector<F32> vertexData;

    F32 deltaAngleMajor = 2 * M_PI / numMajorSegments;
    F32 deltaAngleMinor = 2 * M_PI / numMajorSegments;
    for (U32 i = 0; i < numMajorSegments; ++i)
    {
        F32 angleMajorStart = i * deltaAngleMajor;
        F32 angleMajorEnd = (i + 1) * deltaAngleMajor;

        F32 xMajorStart = std::cos(angleMajorStart);
        F32 yMajorStart = std::sin(angleMajorStart);

        F32 xMajorEnd = std::cos(angleMajorEnd);
        F32 yMajorEnd = std::sin(angleMajorEnd);

        for (U32 j = 0; j < numMinorSegments; ++j)
        {
            F32 angleMinorStart = j * deltaAngleMinor;
            F32 angleMinorEnd = (j + 1) * deltaAngleMinor;

            F32 xMinorStart = std::cos(angleMinorStart);
            F32 yMinorStart = std::sin(angleMinorStart);

            F32 xMinorEnd = std::cos(angleMinorEnd);
            F32 yMinorEnd = std::sin(angleMinorEnd);

            std::array<std::array<F32, 3>, 4> points;
            std::array<std::array<F32, 3>, 4> normals;
            points[0][0] = (xMajorStart + xMajorStart * xMinorStart * radiusMinor) * radiusMajor;
            points[0][1] = (yMajorStart + yMajorStart * xMinorStart * radiusMinor) * radiusMajor;
            points[0][2] = yMinorStart * radiusMinor;
            normals[0][0] = xMajorStart * xMinorStart;
            normals[0][1] = yMajorStart * xMinorStart;
            normals[0][2] = yMinorStart;

            points[1][0] = (xMajorStart + xMajorStart * xMinorEnd * radiusMinor) * radiusMajor;
            points[1][1] = (yMajorStart + yMajorStart * xMinorEnd * radiusMinor) * radiusMajor;
            points[1][2] = yMinorEnd * radiusMinor;
            normals[1][0] = xMajorStart * xMinorEnd;
            normals[1][1] = yMajorStart * xMinorEnd;
            normals[1][2] = yMinorEnd;

            points[2][0] = (xMajorEnd + xMajorEnd * xMinorStart * radiusMinor) * radiusMajor;
            points[2][1] = (yMajorEnd + yMajorEnd * xMinorStart * radiusMinor) * radiusMajor;
            points[2][2] = yMinorStart * radiusMinor;
            normals[2][0] = xMajorEnd * xMinorStart;
            normals[2][1] = yMajorEnd * xMinorStart;
            normals[2][2] = yMinorStart;

            points[3][0] = (xMajorEnd + xMajorEnd * xMinorEnd * radiusMinor) * radiusMajor;
            points[3][1] = (yMajorEnd + yMajorEnd * xMinorEnd * radiusMinor) * radiusMajor;
            points[3][2] = yMinorEnd * radiusMinor;
            normals[3][0] = xMajorEnd * xMinorEnd;
            normals[3][1] = yMajorEnd * xMinorEnd;
            normals[3][2] = yMinorEnd;

            for (U32 k = 0; k < 3; ++k)
            {
                vertexData.push_back(points[k][0]);
                vertexData.push_back(points[k][1]);
                vertexData.push_back(points[k][2]);
                vertexData.push_back(normals[k][0]);
                vertexData.push_back(normals[k][1]);
                vertexData.push_back(normals[k][2]);
            }
            for (U32 k = 3; k > 0; --k)
            {
                vertexData.push_back(points[k][0]);
                vertexData.push_back(points[k][1]);
                vertexData.push_back(points[k][2]);
                vertexData.push_back(normals[k][0]);
                vertexData.push_back(normals[k][1]);
                vertexData.push_back(normals[k][2]);
            }
        }
    }
    return vertexData;
}



int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Setup Render Context ---------------------

    ContextManager& contextManager = ContextManager::Instance();
    RenderWindow renderWindow(contextManager);
    renderWindow.SetTitle("Diffuse per vertex shading");
    renderWindow.Initialize();


    Vector<F32> vertexData = CreateTorusMesh(2.f, 0.5f, 80, 80);

    // Shader Code ------------------------------

    const char vertexShaderCode[] = R"glsl(
            #version 430

            layout (location=0) in vec3 VertexPosition;
            layout (location=1) in vec3 VertexNormals;

            uniform mat4 ProjectionMatrix;
            uniform mat4 ModelWorldMatrix;

            out vec3 lightIntensity;

            void main(void)
            {
                vec4 lightPosition = vec4(-300,-300,-500,1);
                vec3 Kd = vec3(0.0,0.0,1.0);
                vec3 Ld = vec3(1.0,1.0,1.0);

                vec3 normal = normalize(VertexNormals);
                vec4 positionWorld = ModelWorldMatrix * vec4(VertexPosition, 1.0);

                vec3 s = normalize(vec3(lightPosition - positionWorld));

                lightIntensity = Ld * Kd * max(dot(s,normal),0.);

                gl_Position = ProjectionMatrix * positionWorld;
            }
            )glsl";

    const char fragmentShaderCode[] = R"glsl(
            #version 430

            in vec3 lightIntensity;
            out vec4 FragColor;

            void main(void)
            {
                FragColor = vec4(lightIntensity,1.0);
            }
            )glsl";



    // Create Shader and Program ################

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    Program program(vertexShader, fragmentShader);



    // Create Buffer Objects ####################
    BufferObject vertexBuffer(vertexData, GL_STATIC_DRAW);



    // Create Vertex Array Object ###############
    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, vertexBuffer, GL_FLOAT, 3, 24);
    vao.EnableAttribute(1, 0, GL_FLOAT, 3, 12);



    // Bind program and vertex array object #####
    vao.Bind();
    program.Use();
    glClearColor(0.5, 0.5, 0.5, 1.0);


    GLint uniformProjection = program.QueryUniformLocation("ProjectionMatrix");
    GLint uniformModelWorld = program.QueryUniformLocation("ModelWorldMatrix");



    // Start main loop ##########################
    while (renderWindow.IsOpen())
    {
        contextManager.PollEvents();

        Mat4f ProjectionMatrix =
                TransformationMatrix4::OrthogonalProjection(renderWindow.GetWidth(), renderWindow.GetHeight());
        Mat4f ModelWorldMatrix = TransformationMatrix4::Scale(100.f, 100.f, 1.f);
        program.SetUniform(static_cast<I32>(uniformProjection), ProjectionMatrix);
        program.SetUniform(static_cast<I32>(uniformModelWorld), ModelWorldMatrix);

        glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 6);

        renderWindow.SwapBuffers();
    }


    exit(EXIT_SUCCESS);
}
