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
    EXCEPTION(mInitialized, "Context is already initialized");

    glutInit(&argc, &argv);

    glutInitContextVersion(4, 6);
    glutInitContextFlags(mGlutContextFlags);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);


    mInitialized = true;
}



bool ContextGLUT::IsInitialized() const
{
    return mInitialized;
}



void ContextGLUT::EnableDebug()
{
    EXCEPTION(mInitialized, "Context is already initialized. Can not enable debug mode.");
    glEnable(GL_DEBUG_OUTPUT);
    mGlutContextFlags |= GLUT_DEBUG;
    mDebug = true;
}



void ContextGLUT::SetDefaultDebugCallback() const
{
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    glDebugMessageCallback(DefaultDebugCallback, nullptr);
}



void ContextGLUT::DefaultDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                       const GLchar* message, const void* userParam)
{
    // TODO: Write DebugMessageHandler class and make it a member of the context. Move callback there. Then I can also
    // store in a variable if the default callback was set. Debug handler can set different options how debug messages
    // should be treated. Throw, write to log etc...
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        //       THROW(message);

    default:
        std::cout << message << std::endl;
        break;
    }
    U32 test = id;
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



} // namespace GDL::OpenGL
