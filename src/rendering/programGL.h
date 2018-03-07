#pragma once

#include "rendering/shaderGLSL.h"

#include <functional>
#include <map>

namespace GDL
{
class ProgramGL
{
    GLuint mHandle = 0;
    std::map<GLenum, std::reference_wrapper<const ShaderGLSL>> mShader;


public:
    ProgramGL() = delete;
    ProgramGL(const ProgramGL&) = delete; // don't use default -> dtor calls glDeleteProgram
    ProgramGL(ProgramGL&&) = delete; // don't use default -> dtor calls glDeleteProgram
    ProgramGL& operator=(const ProgramGL&) = delete; // don't use default -> dtor calls glDeleteProgram
    ProgramGL& operator=(ProgramGL&&) = delete; // don't use default -> dtor calls glDeleteProgram
    ~ProgramGL();

    template <typename... Args>
    ProgramGL(const Args&... shaderList);

    ProgramGL(std::initializer_list<std::reference_wrapper<const ShaderGLSL>> shaderList);

    //! @brief Gets the programs handle
    //! @return Program handle
    GLuint GetHandle() const
    {
        return mHandle;
    }

private:
    void CheckLinkStatus();
    void Initialize(std::initializer_list<std::reference_wrapper<const ShaderGLSL>> shaderList);
};
}


template <typename... Args>
GDL::ProgramGL::ProgramGL(const Args&... shaderList)
{
    Initialize({std::forward<const Args&>(shaderList)...});
}
