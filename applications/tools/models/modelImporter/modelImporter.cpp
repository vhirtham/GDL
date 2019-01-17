#include "src/openGLWidget.h"
#include "src/mainWindow.h"

#include "gdl/base/container/vector.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"


#include <QApplication>
#include <QOpenGLWidget>

#include <iostream>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1600, 900);
    OpenGLWidget ogl(&w);
    ogl.resize(1000, 800);
    w.show();

    // Shader Code ##############################

    const char vertexShaderCode[] = R"glsl(
            #version 430

            layout (location=0) in vec3 VertexPosition;
            layout (location=1) in vec3 VertexColor;

            out vec3 Color;

            void main(void)
            {
                Color = VertexColor;

                gl_Position = vec4(VertexPosition,1.0);
            }
            )glsl";

    const char fragmentShaderCode[] = R"glsl(
            #version 430

            in vec3 Color;

            out vec4 FragColor;
            void main(void)
            {
                FragColor = vec4(Color,1.0);
            }
            )glsl";

    using namespace GDL;
    using namespace GDL::OpenGL;
    // Create Shader and Program ################

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    Program program(vertexShader, fragmentShader);

    // Create Buffer Objects ####################
    // clang-format off
    Vector<F32> positionData
    {
        -0.8f, -0.8f,  0.0f,
         0.8f, -0.8f,  0.0f,
         0.0f,  0.8f,  0.0f
    };
    Vector<F32> colorData
    {
         1.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  0.0f,  1.0f
    };
    // clang-format on
    BufferObject positionBuffer(positionData, GL_STATIC_DRAW);
    BufferObject colorBuffer(colorData, GL_STATIC_DRAW);


    // Create Vertex Array Object ###############
    VertexArrayObject vao;
    vao.EnableAttribute(0, 0, positionBuffer, GL_FLOAT, 3, 12);
    vao.EnableAttribute(1, 1, colorBuffer, GL_FLOAT, 3, 12);



    // Bind program and vertex array object #####
    vao.Bind();
    program.Use();

    glDrawArrays(GL_TRIANGLES, 0, 3);



    if (glGetError() != GL_NO_ERROR)
        std::cout << "OpenGLError" << std::endl;

    ogl.update();
    return a.exec();
}
