#include "gdl/rendering/openGL/core/contextManagerGLFW.h"

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace GDL::OpenGL
{



ContextManagerGLFW::ContextManagerGLFW()
{
}



ContextManagerGLFW& ContextManagerGLFW::Instance()
{
    static ContextManagerGLFW contextManagerGLFW;
    return contextManagerGLFW;
}



bool ContextManagerGLFW::IsDebug() const
{
    return mDebug;
}



bool ContextManagerGLFW::IsInitialized() const
{
    return mInitialized;
}



void ContextManagerGLFW::EnableDebug()
{
    EXCEPTION(IsInitialized(), "Context is already initialized. Can not enable debug mode.");
    mDebug = true;
}



DebugMessageHandler& ContextManagerGLFW::GetDebugMessageHandler()
{
    return mDebugMessageHandler;
}



void ContextManagerGLFW::PollEvents() const
{
    glfwPollEvents();
}



void ContextManagerGLFW::Initialize([[maybe_unused]] int argc, [[maybe_unused]] char* argv)
{
    I32 initStatus = glfwInit();
    EXCEPTION(initStatus == GL_FALSE, "Could not initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (mDebug)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }

    mInitialized = true;
}



} // namespace GDL::OpenGL
