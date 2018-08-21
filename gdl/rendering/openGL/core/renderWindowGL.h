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

public:
    //! @brief CTor
    RenderWindow(U32 width = 800, U32 height = 600, std::string title = "GDL OpenGL Window");

    //! @brief Initializes the Window
    //! @param argc: Additional argument count
    //! @param argv: Additional arguments
    void Initialize(int argc = 1, char* argv = nullptr);

    //! @brief Sets the title
    //! @param title: New Window Title
    void SetTitle(std::string title);
};
}
}
