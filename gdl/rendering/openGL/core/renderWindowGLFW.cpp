#include "gdl/rendering/openGL/core/renderWindowGLFW.h"

#include "gdl/base/exception.h"
#include "gdl/input/inputState.h"
#include "gdl/input/keyMappingGLFW.h"
#include "gdl/rendering/openGL/core/contextManagerGLFW.h"
#include "gdl/rendering/openGL/core/glewController.h"



#include "gdl/input/key.h"
#include <iostream>

namespace GDL::OpenGL
{


RenderWindowGLFW::RenderWindowGLFW(ContextManagerGLFW& contextManagerGLFW)
    : mContextManagerGLFW{contextManagerGLFW}
{
}



void RenderWindowGLFW::DisableWireframeMode()
{
    DEV_EXCEPTION(!IsInitialized(), "Render window needs to be initialized to disable wire frame mode.");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void RenderWindowGLFW::EnableDepthTest()
{
    DEV_EXCEPTION(!IsInitialized(), "Render window needs to be initialized to enable depth testing.");
    glEnable(GL_DEPTH_TEST);
}



void RenderWindowGLFW::EnableWireframeMode()
{
    DEV_EXCEPTION(!IsInitialized(), "Render window needs to be initialized to enable wire frame mode.");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}



RenderWindowGLFW::~RenderWindowGLFW()
{
    glfwTerminate();
}



void RenderWindowGLFW::Initialize()
{
    EXCEPTION(mInitialized, "Render window is already initialized");

    if (mContextManagerGLFW.IsInitialized() == false)
        mContextManagerGLFW.Initialize();

    mWindow = glfwCreateWindow(static_cast<I32>(mWidth), static_cast<I32>(mHeight), mTitle.c_str(), nullptr, nullptr);
    EXCEPTION(mWindow == nullptr, "Could not create GLFW Window");

    glfwMakeContextCurrent(mWindow);
    glfwSetKeyCallback(mWindow, KeyCallback);
    glfwSetWindowSizeCallback(mWindow, ResizeCallback);


    GLEWController& glewController = GLEWController::Instance();
    if (!glewController.IsInitialized())
        glewController.Initialize();

    if (mContextManagerGLFW.IsDebug())
        mContextManagerGLFW.GetDebugMessageHandler().Initialize();

    mInitialized = true;
}



void RenderWindowGLFW::SetTitle(const char* title)
{
    mTitle = title;
}



U32 RenderWindowGLFW::GetHeight() const
{
    I32 height = 0;
    I32 width = 0;
    glfwGetWindowSize(mWindow, &width, &height);
    return static_cast<U32>(height);
}



U32 RenderWindowGLFW::GetWidth() const
{
    I32 height = 0;
    I32 width = 0;
    glfwGetWindowSize(mWindow, &width, &height);
    return static_cast<U32>(width);
}



bool RenderWindowGLFW::IsInitialized() const
{
    return mInitialized;
}



bool RenderWindowGLFW::IsOpen() const
{
    return !glfwWindowShouldClose(mWindow);
}



void RenderWindowGLFW::SwapBuffers() const
{
    if (IsOpen())
    {
        glfwSwapBuffers(mWindow);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}



void RenderWindowGLFW::KeyCallback([[maybe_unused]] GLFWwindow* window, I32 key, [[maybe_unused]] I32 scancode,
                                   I32 action, [[maybe_unused]] I32 mode)
{
    if (action == GLFW_PRESS)
        Input::InputState::SetKeyPressed(Input::GLFWKeyToGDLKey(key), true);
    else if (action == GLFW_RELEASE)
        Input::InputState::SetKeyPressed(Input::GLFWKeyToGDLKey(key), false);
}



void RenderWindowGLFW::ResizeCallback([[maybe_unused]] GLFWwindow* window, I32 width, I32 height)
{
    glViewport(0, 0, width, height);
}


} // namespace GDL::OpenGL
