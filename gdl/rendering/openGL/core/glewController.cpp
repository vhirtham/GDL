#include "gdl/rendering/openGL/core/glewController.h"

#include "gdl/base/exception.h"
#include "gdl/base/string.h"

#include <GL/glew.h>


namespace GDL::OpenGL
{


GLEWController& GLEWController::Instance()
{
    static GLEWController glewController;
    return glewController;
}



void GLEWController::Initialize()
{
    EXCEPTION(mInitialized, "GLEW is already initialized");

    GLenum GlewInitResult = glewInit();

    EXCEPTION(GLEW_OK != GlewInitResult, MakeString("Could not initialize GLEW! \n GLEW error string: ",
                                                    reinterpret_cast<const char*>(glewGetErrorString(GlewInitResult)))
                                                 .c_str());
    mInitialized = true;
}



bool GLEWController::IsInitialized() const
{
    return mInitialized;
}

} // namespace GDL::OpenGL
