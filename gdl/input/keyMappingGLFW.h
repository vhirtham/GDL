#pragma once

#include "gdl/base/fundamentalTypes.h"



namespace GDL::Input
{

enum class Key;


//! @brief Maps a GLFW keycode to a GDL keycode
//! @param glfwKey: GLFW key code
//! @return GDL keycode
Key GLFWKeyToGDLKey(I32 glfwKey);

} // namespace GDL::Input
