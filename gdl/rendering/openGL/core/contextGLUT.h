#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/mat4.h"

#include <GL/glew.h>
#include <functional>

namespace GDL::OpenGL
{


//! @brief Class that enables and manages the OpenGL 4.3 debug system
class ContextGLUT
{
    bool mInitialized = false;

    ContextGLUT() = default;
public:
    ContextGLUT(const ContextGLUT&) = delete;
    ContextGLUT(ContextGLUT&&) = delete;
    ContextGLUT& operator=(const ContextGLUT&) = delete;
    ContextGLUT& operator=(ContextGLUT&&) = delete;
    ~ContextGLUT() = default;

    //! @brief Initializes the debug context and activates the OpenGL debug system on the render window
    void Initialize(int argc = 1, char* argv = nullptr);


    //! @brief Initializes the context
    //! @param argc: Additional argument count
    //! @param argv: Additional arguments
    static ContextGLUT& Instance();

    //! @brief returns if the context is initialized
    //! @return True / False
    bool IsInitialized() const;
};

} // namespace GDL::OpenGL
