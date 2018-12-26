#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/rendering/openGL/core/debugMessageHandler.h"


namespace GDL::OpenGL
{


//! @brief Singleton class that manages a GLUT context
class ContextManagerGLUT
{
    bool mDebug = false;
    I32 mGlutContextFlags;
    DebugMessageHandler mDebugMessageHandler;


    ContextManagerGLUT();

public:
    ContextManagerGLUT(const ContextManagerGLUT&) = delete;
    ContextManagerGLUT(ContextManagerGLUT&&) = delete;
    ContextManagerGLUT& operator=(const ContextManagerGLUT&) = delete;
    ContextManagerGLUT& operator=(ContextManagerGLUT&&) = delete;
    ~ContextManagerGLUT() = default;

    //! @brief Enables the OpenGL 4.3 debug mechanism
    void EnableDebug();

    //! @brief Gets the debug message handler
    //! @return Debug message handler
    DebugMessageHandler& GetDebugMessageHandler();

    //! @brief Initializes the debug context and activates the OpenGL debug system on the render window
    //! @param argc: Additional argument count
    //! @param argv: Additional arguments
    void Initialize(int argc = 1, char* argv = nullptr);

    //! @brief Gets the singleton instance
    static ContextManagerGLUT& Instance();

    //! @brief Returns if the context is running in debug mode
    //! @return True / False
    bool IsDebug() const;

    //! @brief Returns if the context is initialized
    //! @return True / False
    bool IsInitialized() const;

    //! @brief Polls current events.
    void PollEvents() const;
};

} // namespace GDL::OpenGL
