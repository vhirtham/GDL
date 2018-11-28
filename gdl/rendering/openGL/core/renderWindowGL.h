#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <string>

namespace GDL
{
namespace OpenGL
{
class RenderWindow
{
    U32 mWindowHandle = 0;
    U32 mWidth = 800;
    U32 mHeight = 600;
    std::string mTitle;

    //! @brief CTor
    RenderWindow(U32 width = 800, U32 height = 600, std::string title = "GDL OpenGL Window");

public:
    static RenderWindow& Instance();

    //! @brief Initializes the Window
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
} // namespace OpenGL
} // namespace GDL
