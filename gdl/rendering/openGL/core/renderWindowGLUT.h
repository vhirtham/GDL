#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <string>

namespace GDL::OpenGL
{

class ContextGLUT;

//! @brief This class creates and manages a render window by using thr glut library. It is implemented as a singleton
class RenderWindowGLUT
{
    I32 mWindowHandle = 0;
    U32 mWidth = 800;
    U32 mHeight = 600;
    bool mInitialized = false;
    std::string mTitle = "GDL OpenGL Window";
    ContextGLUT& mContextGLUT;


public:
    RenderWindowGLUT() = delete;
    RenderWindowGLUT(const RenderWindowGLUT& other) = delete;
    RenderWindowGLUT(RenderWindowGLUT&& other) = delete;
    RenderWindowGLUT& operator=(const RenderWindowGLUT& other) = delete;
    RenderWindowGLUT& operator=(RenderWindowGLUT&& other) = delete;
    ~RenderWindowGLUT() = default;

    //! @brief Constructor
    //! @param contextGLUT: Reference to the GLUT context
    RenderWindowGLUT(ContextGLUT& contextGLUT);



    //! @brief Initializes the render window
    //! @param argc: Additional argument count
    //! @param argv: Additional arguments
    void Initialize();

    //! @brief Sets the title
    //! @param title: New Window Title
    void SetTitle(std::string title);

    //! @brief Gets the window height
    //! @return Window height
    U32 GetHeight() const;

    //! @brief Gets the window width
    //! @return Window width
    U32 GetWidth() const;

    //! @brief Returns if the render window is initialized or not
    //! @return True / False
    bool IsInitialized() const;

private:
    //! @brief Callback function that should be called after the window is resized
    //! @param width: New window width
    //! @param height: New window width
    static void ResizeCallback(I32 width, I32 height);
};
} // namespace GDL::OpenGL
