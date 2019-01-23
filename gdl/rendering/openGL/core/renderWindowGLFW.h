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
    U32 mWidth = 1200;
    U32 mHeight = 800;
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

    //! @brief Binds the cursor to the window and hides it
    void CaptureCursor();

    //! @brief Enables depth testing
    void DisableDepthTest();

    //! @brief Enables stencil testing
    void DisableStencilTest();

    //! @brief Disables the wireframe mode.
    void DisableWireframeMode();

    //! @brief Disables writing to the stencil buffer by setting the stencil mask to 0x00
    void DisableWriteToStencilBuffer();

    //! @brief Enables depth testing
    void EnableDepthTest();

    //! @brief Enables stencil testing
    void EnableStencilTest();

    //! @brief Enables the wireframe mode.
    void EnableWireframeMode();

    //! @brief Enables writing to the stencil buffer by setting the stencil mask to 0xFF
    void EnableWriteToStencilBuffer();

    //! @brief Initializes the render window
    void Initialize();

    //! @brief Releases the cursor from the window
    void ReleaseCursor();

    //! @brief Sets the stencil function which determines if a fragment passes the stencil test or not.
    //! @param function: Function enum which defines the comparison operator between the current buffer value and the
    //! provided reference value
    //! @param: referenceValue: Reference value that is compared to the buffer value
    //! @param: mask: Mask that is used to perform an AND operation with buffer and reference value before the stencil
    //! test
    void SetStencilFunction(GLenum function, GLint referenceValue, GLuint mask = 0xFF);

    //! @brief Sets the stencil mask which modifies the value that should be written to the buffer by performing an AND
    //! operation.
    //! @param mask: Bitmask
    void SetStencilMask(GLuint mask);

    //! @brief Sets the stencil test actions
    //! @param actionOnStencilFail: Enum that describes the action when the stencil test fails
    //! @param actionOnDepthFail: Enum that describes the action when the stencil test passes, but the depth test fails
    //! @param actionOnStencilDepthPass: Enum that describes the action when stencil test and depth test pass
    void SetStencilTestAction(GLenum actionOnStencilFail, GLenum actionOnDepthFail, GLenum actionOnStencilDepthPass);

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
    //! @brief Mouse callback function
    //! @param window: Render window
    //! @param mouseX: Mouse x position
    //! @param mouseY: Mouse y position
    static void MouseCallback(GLFWwindow* window, F64 mouseX, F64 mouseY);

    //! @brief Callback function that should be called every time a key event occurs
    //! @param window: Render window
    //! @param key: GLFW key code
    //! @param scancode: Scancode of the key
    //! @param action: Key action
    //! @param mode: Bit code containing the state of modification keys like shift, ctrl etc. See
    //! https://www.glfw.org/docs/latest/group__mods.html
    static void KeyCallback(GLFWwindow* window, I32 key, I32 scancode, I32 action, I32 mode);

    //! @brief Callback function that should be called after the window is resized
    //! @param window: Render window
    //! @param width: New window width
    //! @param height: New window width
    static void ResizeCallback(GLFWwindow* window, I32 width, I32 height);
};
} // namespace GDL::OpenGL
