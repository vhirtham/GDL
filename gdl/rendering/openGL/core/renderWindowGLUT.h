#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/string.h"

namespace GDL::OpenGL
{

class ContextManagerGLUT;

//! @brief This class creates and manages a render window by using thr glut library. It is implemented as a singleton
class RenderWindowGLUT
{
    I32 mWindowHandle = 0;
    U32 mWidth = 800;
    U32 mHeight = 600;
    bool mInitialized = false;
    bool mLockCursor = false;
    String mTitle = "GLUT Window";
    ContextManagerGLUT& mContextManagerGLUT;

    static bool mIsOpen;


public:
    RenderWindowGLUT() = delete;
    RenderWindowGLUT(const RenderWindowGLUT& other) = delete;
    RenderWindowGLUT(RenderWindowGLUT&& other) = delete;
    RenderWindowGLUT& operator=(const RenderWindowGLUT& other) = delete;
    RenderWindowGLUT& operator=(RenderWindowGLUT&& other) = delete;
    ~RenderWindowGLUT() = default;

    //! @brief Constructor
    //! @param contextGLUT: Reference to the GLUT context
    RenderWindowGLUT(ContextManagerGLUT& contextGLUT);

    //! @brief Binds the cursor to the window and hides it
    void CaptureCursor();

    //! @brief Enables depth testing
    void DisableDepthTest();

    //! @brief Enables stencil testing
    void DisableStencilTest();

    //! @brief Disables the wireframe mode.
    void DisableWireframeMode();

    //! @brief Enables depth testing
    void EnableDepthTest();

    //! @brief Enables stencil testing
    void EnableStencilTest();

    //! @brief Enables the wireframe mode.
    void EnableWireframeMode();

    //! @brief Enables writing to the stencil buffer by setting the stencil mask to 0xFF
    void EnableWriteToStencilBuffer();

    //! @brief Initializes the render window
    //! @param argc: Additional argument count
    //! @param argv: Additional arguments
    void Initialize();

    //! @brief Releases the cursor from the window
    void ReleaseCursor();

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
    //! @brief Callback function that should be called after the window is closed
    static void CloseCallback();

    //! @brief Callback function that should be called whenever the display event is triggered
    static void DisplayCallback();

    //! @brief Mouse callback function
    //! @param mouseX: Mouse x position
    //! @param mouseY: Mouse y position
    static void MouseCallback(I32 mouseX, I32 mouseY);

    //! @brief Key down callback
    //! @param key: Keycode
    //! @param mouseX: Mouse x position
    //! @param mouseY: Mouse y position
    static void KeyDownCallback(U8 key, I32 mouseX, I32 mouseY);

    //! @brief Key up callback
    //! @param key: Keycode
    //! @param mouseX: Mouse x position
    //! @param mouseY: Mouse y position
    static void KeyUpCallback(U8 key, I32 mouseX, I32 mouseY);

    //! @brief Callback function that should be called after the window is resized
    //! @param width: New window width
    //! @param height: New window width
    static void ResizeCallback(I32 width, I32 height);
};



} // namespace GDL::OpenGL
