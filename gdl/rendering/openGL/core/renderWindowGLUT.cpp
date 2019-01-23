#include "gdl/rendering/openGL/core/renderWindowGLUT.h"

#include "gdl/base/exception.h"
#include "gdl/input/inputState.h"
#include "gdl/input/keyMappingGLUT.h"
#include "gdl/rendering/openGL/core/contextManagerGLUT.h"
#include "gdl/rendering/openGL/core/glewController.h"

#include <GL/freeglut.h>


#include <iostream>
namespace GDL::OpenGL
{

bool RenderWindowGLUT::mIsOpen = true;



RenderWindowGLUT::RenderWindowGLUT(ContextManagerGLUT& contextGLUT)
    : mContextManagerGLUT{contextGLUT}
{
}



void RenderWindowGLUT::CaptureCursor()
{
    mLockCursor = true;
    glutSetCursor(GLUT_CURSOR_NONE);
}



void RenderWindowGLUT::DisableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
}



void RenderWindowGLUT::DisableStencilTest()
{
    glDisable(GL_STENCIL_TEST);
}



void RenderWindowGLUT::DisableWireframeMode()
{
    DEV_EXCEPTION(!IsInitialized(), "Render window needs to be initialized to disable wire frame mode.");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void RenderWindowGLUT::EnableDepthTest()
{
    DEV_EXCEPTION(!IsInitialized(), "Render window needs to be initialized to enable depth testing.");
    glEnable(GL_DEPTH_TEST);
}



void RenderWindowGLUT::EnableStencilTest()
{
    glEnable(GL_STENCIL_TEST);
}



void RenderWindowGLUT::EnableWireframeMode()
{
    DEV_EXCEPTION(!IsInitialized(), "Render window needs to be initialized to enable wire frame mode.");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}



void RenderWindowGLUT::Initialize()
{
    EXCEPTION(mInitialized, "Render window is already initialized");


    if (!mContextManagerGLUT.IsInitialized())
        mContextManagerGLUT.Initialize();

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitWindowSize(static_cast<I32>(mWidth), static_cast<I32>(mHeight));


    mWindowHandle = glutCreateWindow(mTitle.c_str());
    EXCEPTION(mWindowHandle < 1, "Could not create GLUT Window!");

    glutReshapeFunc(ResizeCallback);
    glutCloseFunc(CloseCallback);
    glutDisplayFunc(DisplayCallback);
    glutKeyboardFunc(KeyDownCallback);
    glutKeyboardUpFunc(KeyUpCallback);
    glutMotionFunc(MouseCallback);
    glutPassiveMotionFunc(MouseCallback);

    GLEWController& glewController = GLEWController::Instance();
    if (!glewController.IsInitialized())
        glewController.Initialize();

    if (mContextManagerGLUT.IsDebug())
        mContextManagerGLUT.GetDebugMessageHandler().Initialize();

    mInitialized = true;
}



void RenderWindowGLUT::ReleaseCursor()
{
    mLockCursor = false;
    glutSetCursor(GLUT_CURSOR_INHERIT);
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



bool RenderWindowGLUT::IsOpen() const
{
    return mIsOpen;
}



void RenderWindowGLUT::SwapBuffers() const
{
    if (IsOpen())
    {
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}



void RenderWindowGLUT::CloseCallback()
{
    mIsOpen = false;
}



void RenderWindowGLUT::DisplayCallback()
{
    glutPostRedisplay();
}



void RenderWindowGLUT::MouseCallback(I32 mouseX, I32 mouseY)
{
    Input::InputState::SetMousePosition(static_cast<F64>(mouseX), static_cast<F64>(mouseY));
}



void RenderWindowGLUT::KeyDownCallback(U8 key, [[maybe_unused]] I32 mouseX, [[maybe_unused]] I32 mouseY)
{
    Input::InputState::SetKeyPressed(Input::GLUTKeyToGDLKey(key), true);
}



void RenderWindowGLUT::KeyUpCallback(U8 key, [[maybe_unused]] I32 mouseX, [[maybe_unused]] I32 mouseY)
{
    Input::InputState::SetKeyPressed(Input::GLUTKeyToGDLKey(key), false);
}



void RenderWindowGLUT::ResizeCallback(I32 width, I32 height)
{
    glViewport(0, 0, width, height);
}


} // namespace GDL::OpenGL
