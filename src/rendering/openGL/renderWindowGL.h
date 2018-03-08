#pragma once

#include "GDLTypedefs.h"

#include <string>

namespace GDL
{
class RenderWindowGL
{
    U32 mWindowHandle = 0;
    U32 mWidth = 800;
    U32 mHeight = 600;
    std::string mTitle;

public:
    //! @brief CTor
    RenderWindowGL(U32 width = 800, U32 height = 600, std::string title = "GDL OpenGL Window");

    //! @brief Initializes the Window
    void Initialize();

    //! @brief Sets the title
    //! @param title: New Window Title
    void SetTitle(std::string title);
};
}
