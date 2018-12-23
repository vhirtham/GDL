#include "gdl/rendering/openGL/core/renderWindowGLFW.h"

#include "gdl/rendering/openGL/core/contextManagerGLFW.h"
#include "gdl/rendering/openGL/core/glewController.h"
#include "gdl/base/exception.h"



namespace GDL::OpenGL
{


RenderWindowGLFW::RenderWindowGLFW(ContextManagerGLFW& contextManagerGLFW)
    : mContextManagerGLFW{contextManagerGLFW}
{
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



bool RenderWindowGLFW::IsInitialized() const
{
    return mInitialized;
}


} // namespace GDL::OpenGL
