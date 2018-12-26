#pragma once

#if (defined(GLFW_FOUND) && !defined(USE_GLUT))
#include "gdl/rendering/openGL/core/renderWindowGLFW.h"
namespace GDL::OpenGL
{
using RenderWindow = RenderWindowGLFW;
}

#elif defined(GLUT_FOUND)
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"
namespace GDL::OpenGL
{
using RenderWindow = RenderWindowGLUT;
}

#endif
