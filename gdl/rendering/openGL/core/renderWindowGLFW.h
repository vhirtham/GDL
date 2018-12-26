#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/string.h"


#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace GDL::OpenGL
{

class ContextManagerGLFW;

//! @brief This class creates and manages a render window by using the GLFW library. It is implemented as a singleton
class RenderWindowGLFW
{
    GLFWwindow* mWindow;
    U32 mWidth = 800;
    U32 mHeight = 600;
    bool mInitialized = false;
    String mTitle = "GLFW Window";
    ContextManagerGLFW& mContextManagerGLFW;


public:
    RenderWindowGLFW() = delete;
    RenderWindowGLFW(const RenderWindowGLFW& other) = delete;
    RenderWindowGLFW(RenderWindowGLFW&& other) = delete;
    RenderWindowGLFW& operator=(const RenderWindowGLFW& other) = delete;
    RenderWindowGLFW& operator=(RenderWindowGLFW&& other) = delete;
    ~RenderWindowGLFW();

    //! @brief Constructor
    //! @param contextManagerGLFW: Reference to the GLFW context manager
    RenderWindowGLFW(ContextManagerGLFW& contextManagerGLFW);



    //! @brief Initializes the render window
    void Initialize();

    //! @brief Sets the title
    //! @param title: New Window Title
    void SetTitle(const char* title);

    //! @brief Gets the window height
    //! @return Window height
    U32 GetHeight() const;

    //! @brief Gets the window width
    //! @return Window width
    U32 GetWidth() const;

    //! @brief Returns if the render window is initialized or not
    //! @return True / False
    bool IsInitialized() const;

    //! @brief Returns if the render window is open
    //! @return True / False
    bool IsOpen() const;

    //! @brief Swaps the buffers
    void SwapBuffers() const;

private:
    //! @brief Callback function that should be called after the window is resized
    //! @param window: Render window
    //! @param width: New window width
    //! @param height: New window width
    static void ResizeCallback(GLFWwindow* window, I32 width, I32 height);
};
} // namespace GDL::OpenGL
