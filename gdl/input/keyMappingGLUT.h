#pragma once

#include "gdl/base/fundamentalTypes.h"



namespace GDL::Input
{

enum class Key;


//! @brief Maps a GLUT keycode to a GDL keycode
//! @param glutKey: GLUT key code
//! @return GDL keycode
Key GLUTKeyToGDLKey(I32 glutKey);

} // namespace GDL::Input
