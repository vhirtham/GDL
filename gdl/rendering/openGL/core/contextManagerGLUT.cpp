#include "gdl/rendering/openGL/core/contextManagerGLUT.h"

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"

#include <GL/freeglut.h>

#include <iostream>

namespace GDL::OpenGL
{



ContextManagerGLUT::ContextManagerGLUT()
    : mGlutContextFlags{GLUT_FORWARD_COMPATIBLE}
{
}



void ContextManagerGLUT::Initialize(int argc, char* argv)
{
    EXCEPTION(IsInitialized(), "Context is already initialized");

    glutInit(&argc, &argv);

    glutInitContextVersion(4, 6);
    glutInitContextFlags(mGlutContextFlags);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
}



bool ContextManagerGLUT::IsInitialized() const
{
    return glutGet(GLUT_INIT_STATE);
}



void ContextManagerGLUT::EnableDebug()
{
    EXCEPTION(IsInitialized(), "Context is already initialized. Can not enable debug mode.");
    glEnable(GL_DEBUG_OUTPUT);
    mGlutContextFlags |= GLUT_DEBUG;
    mDebug = true;
}



DebugMessageHandler& ContextManagerGLUT::GetDebugMessageHandler()
{
    return mDebugMessageHandler;
}



ContextManagerGLUT& ContextManagerGLUT::Instance()
{
    static ContextManagerGLUT contextGLUT;
    return contextGLUT;
}



bool ContextManagerGLUT::IsDebug() const
{
    return mDebug;
}



void ContextManagerGLUT::PollEvents() const
{
    glutMainLoopEvent();
}


} // namespace GDL::OpenGL
