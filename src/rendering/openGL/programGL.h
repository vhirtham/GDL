#pragma once

#include "GDLTypedefs.h"
#include "base/Exception.h"
#include "rendering/openGL/shaderGL.h"

#include <functional>
#include <map>

namespace GDL
{
class ProgramGL
{
    GLuint mHandle = 0;
    std::map<GLenum, std::reference_wrapper<const ShaderGLSL>> mShader;
    std::map<std::string,GLuint> mUniforms;


public:
    ProgramGL() = delete;
    ProgramGL(const ProgramGL&) = delete; // don't use default -> dtor calls glDeleteProgram
    ProgramGL(ProgramGL&&) = delete; // don't use default -> dtor calls glDeleteProgram
    ProgramGL& operator=(const ProgramGL&) = delete; // don't use default -> dtor calls glDeleteProgram
    ProgramGL& operator=(ProgramGL&&) = delete; // don't use default -> dtor calls glDeleteProgram
    ~ProgramGL();

    //! @brief Constructor that takes as many shaders as desired and links the program
    //! @tparam TShader: Varying number of shaders
    //! @param shaderList: List of shaders
    //! @remark Even though the template should be callable with any input type, only the shaderGL class will compile.
    //! Didn't find a better way to use a parameter pack.
    template <typename... TShader>
    ProgramGL(const TShader&... shaderList);


    //! @brief Constructor that takes as many shaders as desired and links the program (initializer version)
    //! @param shaderList: List of shaders
    ProgramGL(std::initializer_list<std::reference_wrapper<const ShaderGLSL>> shaderList);

    //! @brief Gets the programs handle
    //! @return Program handle
    GLuint GetHandle() const
    {
        return mHandle;
    }

    //! @brief Gets the handle of an uniform
    //! @param uniformName: Name of the uniform
    //! @return Handle of the uniform
    GLuint GetUniformHandle(std::string uniformName);

    //! @brief Sets the value of an uniform
    //! @param uniformHandle: handle of the uniform
    //! @param value: New value
    template <typename TScalar>
    void SetUniformScalar(GLuint uniformHandle, TScalar value);

    //! @brief Sets the value of an uniform
    //! @param uniformName: Name of the uniform
    //! @param value: New value
    template <typename TScalar>
    void SetUniformScalar(std::string uniformName, TScalar value);

private:

    //! @brief Checks if the program links without errors
    void CheckLinkStatus();

    //! @brief Links all the passed shaders to the program
    //! @param shaderList: Shaders that should be linked
    void Initialize(std::initializer_list<std::reference_wrapper<const ShaderGLSL>> shaderList);

    //! @brief Finds all the programs uniforms and stores their names and handles.
    void FindUniforms();
};

}


template<typename TScalar>
void GDL::ProgramGL::SetUniformScalar(GLuint uniformHandle, TScalar value)
{
    throw Exception(__PRETTY_FUNCTION__,"Type not supported!");
}


template<typename TScalar>
void GDL::ProgramGL::SetUniformScalar(std::string uniformName, TScalar value)
{
    SetUniformScalar(GetUniformHandle(uniformName),value);
}

template <typename... TShader>
GDL::ProgramGL::ProgramGL(const TShader&... shaderList)
{
    Initialize({std::forward<const TShader&>(shaderList)...});
}
