#pragma once

#include "gdl/rendering/openGL/core/contextManager.h"
#include "gdl/rendering/openGL/core/renderWindow.h"

namespace GDL::OpenGL
{

//! @brief Creates a GLUT render window and applies a default setup for tests
//! @return GLUT render window
RenderWindow& GetRenderWindow()
{
    ContextManager& contextManager = ContextManager::Instance();
    static RenderWindow renderWindow(contextManager);

    if (!renderWindow.IsInitialized())
    {
        contextManager.EnableDebug();
        contextManager.GetDebugMessageHandler().SetOutputMethod(OutputMethod::COUT);
        renderWindow.Initialize();
    }
    return renderWindow;
}
} // namespace GDL::OpenGL
