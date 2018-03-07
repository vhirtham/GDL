#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Tutorials to go on: http://openglbook.com/chapter-1-getting-started.html

#include "rendering/programGL.h"
#include "rendering/renderWindow.h"
#include "rendering/shaderGLSL.h"


using namespace GDL;


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

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    glGenBuffers(1, &VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
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

    RenderWindowGL renderWindow;
    renderWindow.SetTitle("OpenGL Triangle Test");
    renderWindow.Initialize();



    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Exit);



    // fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));
    const GLchar* VertexShaderCode = {"#version 400\n"

                                      "layout(location=0) in vec4 in_Position;\n"
                                      "layout(location=1) in vec4 in_Color;\n"
                                      "layout(location=2) in vec2 in_TexCoord;\n"
                                      "out vec4 ex_Color;\n"
                                      "out vec2 ex_TexCoord;\n"

                                      "void main(void)\n"
                                      "{\n"
                                      "  gl_Position = in_Position;\n"
                                      "  ex_Color = in_Color;\n"
                                      " ex_TexCoord = in_TexCoord;\n"
                                      "}\n"};



    //    const GLchar* FragmentShaderCode = {"#version 400\n"

    //                                        "in vec4 ex_Color;\n"
    //                                        "out vec4 out_Color;\n"

    //                                        "void main(void)\n"
    //                                        "{\n"
    //                                        "  out_Color = ex_Color;\n"
    //                                        "}\n"};

    const GLchar* FragmentShaderCode = {"#version 400\n"

                                        "in vec4 ex_Color;\n"
                                        "in vec2 ex_TexCoord;\n"
                                        "out vec4 out_Color;\n"

                                        "void main(void)\n"
                                        "{\n"
                                        "float factor = 6.28 * 40;"
                                        "float y = ex_TexCoord.y * factor;\n"
                                        "float dy = dFdy(ex_TexCoord.y) * 6.28 * factor;\n"
                                        "dy = clamp(dy,0.,6.28);"
                                        "float y1 = y - dy*0.5;\n"
                                        "float y2 = y + dy*0.5;\n"
                                        "float f1 = (sin(y)+1)*0.5;\n"
                                        "float f2 = (-cos(y2)+y2 + cos(y1)-y1)*0.5/dy;\n"
                                        "  out_Color = vec4(ex_Color.xyz,f2);\n"
                                        "}\n"};

    ShaderGLSL vertexShader(GL_VERTEX_SHADER, VertexShaderCode);
    ShaderGLSL fragmentShader(GL_FRAGMENT_SHADER, FragmentShaderCode);

    ProgramGL program(vertexShader, fragmentShader);
    glUseProgram(program.GetHandle());

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(stderr, "ERROR: Could not create the shaders: %s \n", gluErrorString(ErrorCheckValue));

        exit(-1);
    }

    CreateVBO();
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glutShowWindow();

    while (KeepRunning())
    {
        glutMainLoopEvent();
    }
    // glutMainLoop(); // Alternative: glutMainLoopEvent
    // glutMainLoopEvent();
    exit(EXIT_SUCCESS);
}
