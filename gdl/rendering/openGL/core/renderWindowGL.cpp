#include "renderWindowGL.h"
#include "gdl/base/exception.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

GDL::OpenGL::RenderWindow::RenderWindow(U32 width, U32 height, std::string title)
    : mWidth(width)
    , mHeight(height)
    , mTitle(title)
{
}

GDL::OpenGL::RenderWindow& GDL::OpenGL::RenderWindow::Instance()
{
    static RenderWindow renderWindow = RenderWindow(1000, 800, "RenderWindowGLUT");
    return renderWindow;
}

void GDL::OpenGL::RenderWindow::Initialize(int argc, char* argv)
{
    glutInit(&argc, &argv);

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

    glutReshapeFunc(ResizeCallback);
}

void GDL::OpenGL::RenderWindow::SetTitle(std::string title)
{
    mTitle = title;
}

GDL::U32 GDL::OpenGL::RenderWindow::GetHeight() const
{
    return mHeight;
}

GDL::U32 GDL::OpenGL::RenderWindow::GetWidth() const
{
    return mWidth;
}

void GDL::OpenGL::RenderWindow::ResizeCallback(I32 width, I32 height)
{
    RenderWindow& renderWindow = Instance();
    renderWindow.mWidth = static_cast<U32>(width);
    renderWindow.mHeight = static_cast<U32>(height);
    glViewport(0, 0, width, height);
}
