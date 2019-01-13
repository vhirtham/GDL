#pragma once

#include "gdl/rendering/openGL/core/contextManager.h"
#include "gdl/rendering/openGL/core/renderWindow.h"


#include <utility>

namespace GDL::OpenGL
{

//! @brief Creates a default context and render window
//! @return Pair containing a reference to the render window and the context
std::pair<RenderWindow&, ContextManager&> DefaultRenderSetup()
{
    ContextManager& contextManager = ContextManager::Instance();
    contextManager.EnableDebug();
    static RenderWindow renderWindow(contextManager);
    renderWindow.Initialize();
    renderWindow.EnableDepthTest();

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return std::pair<RenderWindow&, ContextManager&>(renderWindow, contextManager);
}

} // namespace GDL::OpenGL
