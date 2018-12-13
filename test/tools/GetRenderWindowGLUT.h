#pragma once

#include "gdl/rendering/openGL/core/contextGLUT.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"

namespace GDL::OpenGL
{

//! @brief Creates a GLUT render window and applies a default setup for tests
//! @return GLUT render window
RenderWindowGLUT& GetRenderWindowGLUT()
{
    ContextGLUT& contextGLUT = ContextGLUT::Instance();
    static RenderWindowGLUT renderWindow(contextGLUT);

    if (!renderWindow.IsInitialized())
    {
        contextGLUT.EnableDebug();
        contextGLUT.GetDebugMessageHandler().SetOutputMethod(GL_DEBUG_TYPE_ERROR, OutputMethod::COUT);
        renderWindow.Initialize();
    }
    return renderWindow;
}
} // namespace GDL::OpenGL
