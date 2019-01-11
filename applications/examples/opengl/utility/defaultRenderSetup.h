#pragma once

#include "gdl/rendering/openGL/core/contextManager.h"
#include "gdl/rendering/openGL/core/renderWindow.h"


#include <utility>

namespace GDL::OpenGL
{
std::pair<RenderWindow&, ContextManager&> DefaultRenderSetup()
{
    ContextManager& contextManager = ContextManager::Instance();
    contextManager.EnableDebug();
    static RenderWindow renderWindow(contextManager);
    renderWindow.Initialize();
    renderWindow.EnableDepthTest();

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return std::pair<RenderWindow&, ContextManager&>(renderWindow, contextManager);
}

} // namespace GDL::OpenGL
