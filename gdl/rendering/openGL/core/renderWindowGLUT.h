#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <string>

namespace GDL::OpenGL
{

//! @brief This class creates and manages a render window by using thr glut library. It is implemented as a singleton
class RenderWindowGLUT
{
    I32 mWindowHandle = 0;
    U32 mWidth = 800;
    U32 mHeight = 600;
    std::string mTitle = "GDL OpenGL Window";

    RenderWindowGLUT() = default;
public:
    RenderWindowGLUT(const RenderWindowGLUT& other) = delete;
    RenderWindowGLUT(RenderWindowGLUT&& other) = delete;
    RenderWindowGLUT& operator=(const RenderWindowGLUT& other) = delete;
    RenderWindowGLUT& operator=(RenderWindowGLUT&& other) = delete;
    ~RenderWindowGLUT() = default;


    //! @brief Gets the render windows instance
    //! @return Render window instance
    static RenderWindowGLUT& Instance();

    //! @brief Initializes the render window
    //! @param argc: Additional argument count
    //! @param argv: Additional arguments
    void Initialize(int argc = 1, char* argv = nullptr);

    //! @brief Sets the title
    //! @param title: New Window Title
    void SetTitle(std::string title);

    //! @brief Gets the window height
    //! @return Window height
    U32 GetHeight() const;

    //! @brief Gets the window width
    //! @return Window width
    U32 GetWidth() const;

private:
    //! @brief Callback function that should be called after the window is resized
    //! @param width: New window width
    //! @param height: New window width
    static void ResizeCallback(I32 width, I32 height);
};
} // namespace GDL::OpenGL
