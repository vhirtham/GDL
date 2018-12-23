#include "gdl/rendering/openGL/core/renderWindowGLUT.h"

#include "gdl/rendering/openGL/core/contextGLUT.h"
#include "gdl/rendering/openGL/core/glewController.h"
#include "gdl/base/exception.h"

#include <GL/freeglut.h>



namespace GDL::OpenGL
{



RenderWindowGLUT::RenderWindowGLUT(ContextGLUT& contextGLUT)
    : mContextGLUT{contextGLUT}
{
}



void RenderWindowGLUT::Initialize()
{
    EXCEPTION(mInitialized, "Render window is already initialized");


    if (!mContextGLUT.IsInitialized())
        mContextGLUT.Initialize();

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitWindowSize(static_cast<I32>(mWidth), static_cast<I32>(mHeight));


    mWindowHandle = glutCreateWindow(mTitle.c_str());
    EXCEPTION(mWindowHandle < 1, "Could not create GLUT Window!");

    glutReshapeFunc(ResizeCallback);

    GLEWController& glewController = GLEWController::Instance();
    if (!glewController.IsInitialized())
        glewController.Initialize();

    if (mContextGLUT.IsDebug())
        mContextGLUT.GetDebugMessageHandler().Initialize();

    mInitialized = true;
}



void RenderWindowGLUT::SetTitle(const char* title)
{
    mTitle = title;
}



U32 RenderWindowGLUT::GetHeight() const
{
    return static_cast<U32>(glutGet(GLUT_WINDOW_HEIGHT));
}



U32 RenderWindowGLUT::GetWidth() const
{
    return static_cast<U32>(glutGet(GLUT_WINDOW_WIDTH));
}



bool RenderWindowGLUT::IsInitialized() const
{
    return mInitialized;
}



void RenderWindowGLUT::ResizeCallback(I32 width, I32 height)
{
    glViewport(0, 0, width, height);
}


} // namespace GDL::OpenGL
