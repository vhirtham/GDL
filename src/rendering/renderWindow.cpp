#include "renderWindow.h"
#include "base/Exception.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

GDL::RenderWindowGL::RenderWindowGL(U32 width, U32 height, std::string title)
    : mWidth(width)
    , mHeight(height)
    , mTitle(title)
{
}

void GDL::RenderWindowGL::Initialize()
{
    int argc = 1;
    char* argv[1] = {(char*)""};
    glutInit(&argc, argv);

    glutInitContextVersion(4, 6);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitWindowSize(mWidth, mHeight);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    mWindowHandle = glutCreateWindow(mTitle.c_str());
    if (mWindowHandle < 1)
        throw Exception(__PRETTY_FUNCTION__, "Could not create OpenGL Window!");

    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
        throw Exception(__PRETTY_FUNCTION__,
                        "Could not initialize GLEW! \n GLEW error string: " +
                                std::string(reinterpret_cast<const char*>(glewGetErrorString(GlewInitResult))));
}

void GDL::RenderWindowGL::SetTitle(std::string title)
{
    mTitle = title;
}
