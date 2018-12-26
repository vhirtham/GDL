#pragma once

#if (defined(GLFW_FOUND) && !defined(USE_GLUT))
#include "gdl/rendering/openGL/core/contextManagerGLFW.h"
namespace GDL::OpenGL
{
using ContextManager = ContextManagerGLFW;
}

#elif defined(GLUT_FOUND)
#include "gdl/rendering/openGL/core/contextManagerGLUT.h"
namespace GDL::OpenGL
{
using ContextManager = ContextManagerGLUT;
}
#endif
