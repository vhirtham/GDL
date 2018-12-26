#include "gdl/rendering/openGL/core/contextGLUT.h"

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"

#include <GL/freeglut.h>

#include <iostream>

namespace GDL::OpenGL
{



ContextGLUT::ContextGLUT()
    : mGlutContextFlags{GLUT_FORWARD_COMPATIBLE}
{
}



void ContextGLUT::Initialize(int argc, char* argv)
{
    EXCEPTION(IsInitialized(), "Context is already initialized");

    glutInit(&argc, &argv);

    glutInitContextVersion(4, 6);
    glutInitContextFlags(mGlutContextFlags);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
}



bool ContextGLUT::IsInitialized() const
{
    return glutGet(GLUT_INIT_STATE);
}



void ContextGLUT::EnableDebug()
{
    EXCEPTION(IsInitialized(), "Context is already initialized. Can not enable debug mode.");
    glEnable(GL_DEBUG_OUTPUT);
    mGlutContextFlags |= GLUT_DEBUG;
    mDebug = true;
}



DebugMessageHandler& ContextGLUT::GetDebugMessageHandler()
{
    return mDebugMessageHandler;
}



ContextGLUT& ContextGLUT::Instance()
{
    static ContextGLUT contextGLUT;
    return contextGLUT;
}



bool ContextGLUT::IsDebug() const
{
    return mDebug;
}



void ContextGLUT::PollEvents() const
{
    glutMainLoopEvent();
}


} // namespace GDL::OpenGL
