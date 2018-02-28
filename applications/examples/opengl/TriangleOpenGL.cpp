#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

void ResizeFunction(int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glutSwapBuffers();
    glutPostRedisplay();
}


int main(int argc, char* argv[])
{
    glewInit();
    glutInit(&argc, argv);
    glutInitContextVersion(4, 0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitWindowSize(800, 600);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    int WindowHandle = glutCreateWindow("WINDOW_TITLE_PREFIX");

    if (WindowHandle < 1)
    {
        fprintf(stderr, "ERROR: Could not create a new rendering window.\n");
        exit(EXIT_FAILURE);
    }
    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);

    fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glutMainLoop();

    exit(EXIT_SUCCESS);
}
