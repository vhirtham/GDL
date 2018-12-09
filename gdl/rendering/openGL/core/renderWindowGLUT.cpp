#include "renderWindowGLUT.h"
#include "gdl/base/exception.h"

#include <GL/glew.h>
#include <GL/freeglut.h>



namespace GDL::OpenGL
{



RenderWindowGLUT& RenderWindowGLUT::Instance()
{
    static RenderWindowGLUT renderWindow = RenderWindowGLUT();
    return renderWindow;
}



void RenderWindowGLUT::Initialize(int argc, char* argv)
{
    glutInit(&argc, &argv);

    glutInitContextVersion(4, 6);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitWindowSize(static_cast<I32>(mWidth), static_cast<I32>(mHeight));
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    mWindowHandle = glutCreateWindow(mTitle.c_str());
    EXCEPTION(mWindowHandle < 1, "Could not create OpenGL Window!");

    GLenum GlewInitResult = glewInit();
    EXCEPTION(GLEW_OK != GlewInitResult,
              "Could not initialize GLEW! \n GLEW error string: " +
                      std::string(reinterpret_cast<const char*>(glewGetErrorString(GlewInitResult))));

    glutReshapeFunc(ResizeCallback);
}



void RenderWindowGLUT::SetTitle(std::string title)
{
    mTitle = title;
}



U32 RenderWindowGLUT::GetHeight() const
{
    return mHeight;
}



U32 RenderWindowGLUT::GetWidth() const
{
    return mWidth;
}



void RenderWindowGLUT::ResizeCallback(I32 width, I32 height)
{
    RenderWindowGLUT& renderWindow = Instance();
    renderWindow.mWidth = static_cast<U32>(width);
    renderWindow.mHeight = static_cast<U32>(height);
    glViewport(0, 0, width, height);
}


} // namespace GDL::OpenGL
