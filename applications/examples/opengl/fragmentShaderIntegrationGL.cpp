
// Tutorials to go on: http://openglbook.com/chapter-1-getting-started.html

#include "gdl/rendering/openGL/core/bufferObjectGL.h"
#include "gdl/rendering/openGL/core/programGL.h"
#include "gdl/rendering/openGL/core/renderWindowGL.h"
#include "gdl/rendering/openGL/core/shaderGL.h"
#include "gdl/rendering/openGL/core/vertexArrayObjectGL.h"
#include "gdl/rendering/openGL/management/programDataManagerGL.h"
#include "gdl/rendering/openGL/management/managedUniformBufferObjectGL.h"

#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace GDL;
using namespace GDL::OpenGL;

bool KeepRunning(bool keepRunningIn = true)
{
    static bool keepRunning = true;
    if (keepRunningIn == false)
        keepRunning = false;
    return keepRunning;
}
void Exit()
{
    KeepRunning(false);
}


void ResizeFunction(int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

void RenderFunction()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
    glutPostRedisplay();
}
void DestroyVBO(GLuint VaoId, GLuint VboId, GLuint ColorBufferId)
{
    GLenum ErrorCheckValue = glGetError();

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ColorBufferId);
    glDeleteBuffers(1, &VboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(stderr, "ERROR: Could not destroy the VBO: %s \n", gluErrorString(ErrorCheckValue));

        exit(-1);
    }
}

void DestroyShaders(GLuint VertexShaderId, GLuint FragmentShaderId, GLuint ProgramId)
{
    GLenum ErrorCheckValue = glGetError();

    glUseProgram(0);

    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);


    glDeleteProgram(ProgramId);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(stderr, "ERROR: Could not destroy the shaders: %s \n", gluErrorString(ErrorCheckValue));

        exit(-1);
    }
}
void Cleanup(GLuint VaoId, GLuint VboId, GLuint ColorBufferId, GLuint VertexShaderId, GLuint FragmentShaderId,
             GLuint ProgramId)
{
    DestroyShaders(VertexShaderId, FragmentShaderId, ProgramId);
    DestroyVBO(VaoId, VboId, ColorBufferId);
}
void CreateVBO(void)
{
    GLuint VaoId;
    GLuint VboId;
    GLuint ColorBufferId;
    GLuint TexCoordId;


    GLfloat Vertices[] = {-0.8f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.8f, -1.0f, 0.0f, 1.0f};

    GLfloat Colors[] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    GLfloat TexCoords[] = {0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f};

    GLenum ErrorCheckValue = glGetError();

    glGenBuffers(1, &VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ColorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &TexCoordId);
    glBindBuffer(GL_ARRAY_BUFFER, TexCoordId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords), TexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(stderr, "ERROR: Could not create a VBO: %s \n", gluErrorString(ErrorCheckValue));

        exit(-1);
    }
}


int main(int argc, char* argv[])
{
    GLenum ErrorCheckValue = glGetError();

    RenderWindow renderWindow;
    renderWindow.SetTitle("OpenGL Triangle Test");
    renderWindow.Initialize();



    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Exit);



    // fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));
    const GLchar* VertexShaderCode = {"#version 430\n"

                                      "layout(std140,binding=3) uniform GlobalMatrices"
                                      "{"
                                      " mat4 perspectiveMatrix;"
                                      " mat4 translationMatrix[2];"
                                      "float values[15];"
                                      "float test;"
                                      "};"
                                      "layout(location=0) in vec4 in_Position;\n"
                                      "layout(location=1) in vec4 in_Color;\n"
                                      "layout(location=2) in vec2 in_TexCoord;\n"
                                      "out vec4 ex_Color;\n"
                                      "out vec2 ex_TexCoord;\n"

                                      "void main(void)\n"
                                      "{\n"
                                      "  gl_Position = perspectiveMatrix*in_Position;\n"
                                      "  ex_Color = in_Color;\n"
                                      " ex_TexCoord = in_TexCoord;\n"
                                      "}\n"};



    //    const GLchar* FragmentShaderCode = {"#version 430\n"

    //                                        "in vec4 ex_Color;\n"
    //                                        "out vec4 out_Color;\n"

    //                                        "void main(void)\n"
    //                                        "{\n"
    //                                        "  out_Color = ex_Color;\n"
    //                                        "}\n"};

    const GLchar* FragmentShaderCode = {
            "#version 430\n"

            "layout(location=0)uniform float frequency;"
            "uniform float colMod[3];"

            "in vec4 ex_Color;\n"
            "in vec2 ex_TexCoord;\n"
            "out vec4 out_Color;\n"

            "void main(void)\n"
            "{\n"
            "vec4 color = vec4(ex_Color.x*colMod[0],ex_Color.y*colMod[1],ex_Color.z*colMod[2],ex_Color.w);"
            "float factor = 6.28 * frequency;"
            "float y = ex_TexCoord.y * factor;\n"
            "float dy = dFdy(ex_TexCoord.y) * 6.28 * factor;\n"
            "dy = clamp(dy,0.,6.28);"
            "float y1 = y - dy*0.5;\n"
            "float y2 = y + dy*0.5;\n"
            "float f1 = (sin(y)+1)*0.5;\n"
            "float f2 = (-cos(y2)+y2 + cos(y1)-y1)*0.5/dy;\n"
            "  out_Color = vec4(color.xyz,f2);\n"
            "}\n"};

    Shader vertexShader(GL_VERTEX_SHADER, VertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, FragmentShaderCode);

    std::vector<F32> perspectiveMatValues{1.2, 0., 0., 0., 0., 0.9, 0., 0., 0., 0., 1., 0., 0., 0., 0., 1.};

    Program program(vertexShader, fragmentShader);
    ProgramDataManager programDM(program);
    ManagedUniformBufferObject ubo(programDM.GetUniformBlock("GlobalMatrices"), GL_DYNAMIC_DRAW);
    ubo.SetData("perspectiveMatrix", perspectiveMatValues);
    programDM.SetUniform<GL_FLOAT>("frequency", 10.f);
    programDM.SetUniform<GL_FLOAT>("colMod[0]", 1.f);
    programDM.SetUniformArray<GL_FLOAT>("colMod[1]", std::vector<F32>{1.f, 1.f}.data(), 2);

    glUseProgram(program.GetHandle());

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(stderr, "ERROR: Could not create the shaders: %s \n", gluErrorString(ErrorCheckValue));

        exit(-1);
    }

    CreateVBO();

    std::vector<F32> Vertices{-0.8f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.8f, -1.0f, 0.0f, 1.0f};
    std::vector<F32> Colors{1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f};
    std::vector<F32> TexCoords{0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f};
    std::vector<F32> VerticesAndColors{-0.8f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.8f, -1.0f, 0.0f, 1.0f,
                                       1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f};
    BufferObject bufferVertices(48, GL_STATIC_DRAW);
    BufferObject bufferColors(48, GL_STATIC_DRAW);
    BufferObject bufferTexCoords(24, GL_STATIC_DRAW);
    BufferObject bufferVerticesAndColors(96, GL_STATIC_DRAW);
    bufferVertices.SetData(Vertices);
    bufferColors.SetData(Colors);
    bufferTexCoords.SetData(TexCoords);
    bufferVerticesAndColors.SetData(VerticesAndColors);
    VertexArrayObject vao;
    //    vao.AddAttribute(bufferVertices, 4, GL_FLOAT, 16);
    //    vao.AddAttribute(bufferColors, 4, GL_FLOAT, 16);
    vao.EnableAttribute(0, bufferVerticesAndColors, 4, GL_FLOAT, 16);
    // vao.EnableAttribute(1, bufferVerticesAndColors, 4, GL_FLOAT, 16, 48);
    vao.EnableAttribute(1, 4, GL_FLOAT);
    vao.EnableAttribute(2, bufferTexCoords, 2, GL_FLOAT, 8);

    // vao.SetAttributeBuffer(1, bufferColors, 16);
    vao.SetAttributeBuffer(1, bufferVerticesAndColors, 16, 48);

    vao.Bind();

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glutShowWindow();
    std::chrono::system_clock::time_point prevTime = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point currTime = std::chrono::system_clock::now();
    F32 frequency = 1.0;
    F32 factor = 1.;
    while (KeepRunning())
    {
        currTime = std::chrono::system_clock::now();
        F32 delta_t = std::chrono::duration_cast<std::chrono::microseconds>(currTime - prevTime).count();
        frequency += factor * delta_t / (10.e3 * 4);
        if (frequency > 120.)
            factor = -1.0;
        if (frequency < 1.)
            factor = 1.0;
        programDM.SetUniform<GL_FLOAT>("frequency", frequency);
        glutMainLoopEvent();
        prevTime = currTime;
    }
    // glutMainLoop(); // Alternative: glutMainLoopEvent
    // glutMainLoopEvent();
    exit(EXIT_SUCCESS);
}
