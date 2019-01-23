#include "gdl/rendering/openGL/core/renderWindowGLFW.h"

#include "gdl/base/exception.h"
#include "gdl/input/inputState.h"
#include "gdl/input/keyMappingGLFW.h"
#include "gdl/rendering/openGL/core/contextManagerGLFW.h"
#include "gdl/rendering/openGL/core/glewController.h"



namespace GDL::OpenGL
{


RenderWindowGLFW::RenderWindowGLFW(ContextManagerGLFW& contextManagerGLFW)
    : mContextManagerGLFW{contextManagerGLFW}
{
}



void RenderWindowGLFW::CaptureCursor()
{
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}



void RenderWindowGLFW::DisableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
}



void RenderWindowGLFW::DisableStencilTest()
{
    glDisable(GL_STENCIL_TEST);
}



void RenderWindowGLFW::DisableWireframeMode()
{
    DEV_EXCEPTION(!IsInitialized(), "Render window needs to be initialized to disable wire frame mode.");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void RenderWindowGLFW::DisableWriteToStencilBuffer()
{
    SetStencilMask(0x00);
}



void RenderWindowGLFW::EnableDepthTest()
{
    DEV_EXCEPTION(!IsInitialized(), "Render window needs to be initialized to enable depth testing.");
    glEnable(GL_DEPTH_TEST);
}

void RenderWindowGLFW::EnableStencilTest()
{
    glEnable(GL_STENCIL_TEST);
}



void RenderWindowGLFW::EnableWireframeMode()
{
    DEV_EXCEPTION(!IsInitialized(), "Render window needs to be initialized to enable wire frame mode.");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}



void RenderWindowGLFW::EnableWriteToStencilBuffer()
{
    SetStencilMask(0xFF);
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
    glfwSetCursorPosCallback(mWindow, MouseCallback);
    glfwSetWindowSizeCallback(mWindow, ResizeCallback);


    GLEWController& glewController = GLEWController::Instance();
    if (!glewController.IsInitialized())
        glewController.Initialize();

    if (mContextManagerGLFW.IsDebug())
        mContextManagerGLFW.GetDebugMessageHandler().Initialize();

    mInitialized = true;
}



void RenderWindowGLFW::ReleaseCursor()
{
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}



void RenderWindowGLFW::SetStencilFunction(GLenum function, GLint referenceValue, GLuint mask)
{
    glStencilFunc(function, referenceValue, mask);
}



void RenderWindowGLFW::SetStencilMask(GLuint mask)
{
    glStencilMask(mask);
}



void RenderWindowGLFW::SetStencilTestAction(GLenum actionOnStencilFail, GLenum actionOnDepthFail,
                                            GLenum actionOnStencilDepthPass)
{
    glStencilOp(actionOnStencilFail, actionOnDepthFail, actionOnStencilDepthPass);
}



void RenderWindowGLFW::SetTitle(const char* title)
{
    mTitle = title;
    if (mInitialized)
        glfwSetWindowTitle(mWindow, mTitle.c_str());
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
}



void RenderWindowGLFW::MouseCallback([[maybe_unused]] GLFWwindow* window, F64 mouseX, F64 mouseY)
{
    Input::InputState::SetMousePosition(mouseX, mouseY);
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
