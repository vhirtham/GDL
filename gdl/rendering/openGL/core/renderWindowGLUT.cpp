#include "gdl/rendering/openGL/core/renderWindowGLUT.h"

#include "gdl/rendering/openGL/core/contextGLUT.h"
#include "gdl/rendering/openGL/core/GLEWController.h"
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
    if (!mContextGLUT.IsInitialized())
        mContextGLUT.Initialize();

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitWindowSize(static_cast<I32>(mWidth), static_cast<I32>(mHeight));


    mWindowHandle = glutCreateWindow(mTitle.c_str());
    EXCEPTION(mWindowHandle < 1, "Could not create OpenGL Window!");

    glutReshapeFunc(ResizeCallback);

    GLEWController& glewController = GLEWController::Instance();
    if (!glewController.IsInitialized())
        glewController.Initialize();

    mInitialized = true;
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



bool RenderWindowGLUT::IsInitialized() const
{
    return mInitialized;
}



void RenderWindowGLUT::ResizeCallback(I32 width, I32 height)
{
    glViewport(0, 0, width, height);
}


} // namespace GDL::OpenGL
