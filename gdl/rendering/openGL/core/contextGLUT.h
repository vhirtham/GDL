#pragma once


#include "gdl/base/fundamentalTypes.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace GDL::OpenGL
{


//! @brief Singleton class that manages a GLUT context
class ContextGLUT
{
    bool mInitialized = false;
    bool mDebug = false;
    I32 mGlutContextFlags;


    ContextGLUT();

public:
    ContextGLUT(const ContextGLUT&) = delete;
    ContextGLUT(ContextGLUT&&) = delete;
    ContextGLUT& operator=(const ContextGLUT&) = delete;
    ContextGLUT& operator=(ContextGLUT&&) = delete;
    ~ContextGLUT() = default;

    //! @brief Initializes the debug context and activates the OpenGL debug system on the render window
    void Initialize(int argc = 1, char* argv = nullptr);


    //! @brief Initializes the context
    //! @param argc: Additional argument count
    //! @param argv: Additional arguments
    static ContextGLUT& Instance();

    //! @brief Returns if the context is running in debug mode
    //! @return True / False
    bool IsDebug() const;

    //! @brief Returns if the context is initialized
    //! @return True / False
    bool IsInitialized() const;

    //! @brief Enables the OpenGL 4.3 debug mechanism
    void EnableDebug();

    //! Sets the contexts debug callback to the default version
    void SetDefaultDebugCallback() const;

private:
    static void DefaultDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                     const GLchar* message, const void* userParam);
};

} // namespace GDL::OpenGL
