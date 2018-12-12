#include "gdl/rendering/openGL/core/contextGLUT.h"

#include "gdl/base/exception.h"

#include <GL/freeglut.h>

namespace GDL::OpenGL
{



void ContextGLUT::Initialize(int argc, char* argv)
{
    EXCEPTION(mInitialized, "Context is already initialized");

    glutInit(&argc, &argv);

    glutInitContextVersion(4, 6);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    mInitialized = true;
}



bool ContextGLUT::IsInitialized() const
{
    return mInitialized;
}



ContextGLUT& ContextGLUT::Instance()
{
    static ContextGLUT contextGLUT;
    return contextGLUT;
}

} // namespace GDL::OpenGL
