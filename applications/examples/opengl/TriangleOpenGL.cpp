#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Tutorials to go on: http://openglbook.com/chapter-1-getting-started.html

#include "rendering/renderWindow.h"

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
void CreateShaders()
{
    GLenum ErrorCheckValue = glGetError();
    GLuint VertexShaderId;
    GLuint FragmentShaderId;
    GLuint ProgramId;
    const GLchar* VertexShader = {"#version 400\n"

                                  "layout(location=0) in vec4 in_Position;\n"
                                  "layout(location=1) in vec4 in_Color;\n"
                                  "out vec4 ex_Color;\n"

                                  "void main(void)\n"
                                  "{\n"
                                  "  gl_Position = in_Position;\n"
                                  "  ex_Color = in_Color;\n"
                                  "}\n"};



    const GLchar* FragmentShader = {"#version 400\n"

                                    "in vec4 ex_Color;\n"
                                    "out vec4 out_Color;\n"

                                    "void main(void)\n"
                                    "{\n"
                                    "  out_Color = ex_Color;\n"
                                    "}\n"};

    VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
    glCompileShader(VertexShaderId);

    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
    glCompileShader(FragmentShaderId);

    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
    glLinkProgram(ProgramId);
    glUseProgram(ProgramId);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(stderr, "ERROR: Could not create the shaders: %s \n", gluErrorString(ErrorCheckValue));

        exit(-1);
    }
}
void DestroyShaders(GLuint VertexShaderId, GLuint FragmentShaderId, GLuint ProgramId)
{
    GLenum ErrorCheckValue = glGetError();

    glUseProgram(0);

    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);

    glDeleteShader(FragmentShaderId);
    glDeleteShader(VertexShaderId);

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

    GLfloat Vertices[] = {-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.8f, 0.0f, 1.0f, 0.8f, -0.8f, 0.0f, 1.0f};

    GLfloat Colors[] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

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

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(stderr, "ERROR: Could not create a VBO: %s \n", gluErrorString(ErrorCheckValue));

        exit(-1);
    }
}
using namespace GDL;

int main(int argc, char* argv[])
{
    RenderWindowGL renderWindow;
    renderWindow.SetTitle("OpenGL Triangle Test");
    renderWindow.Initialize();



    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Exit);



    // fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

    CreateShaders();
    CreateVBO();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
