#pragma once

#include "gdl/GDLTypedefs.h"

#include <GL/glew.h>
#include <functional>

namespace GDL
{
namespace OpenGL
{
class Shader;

//! @brief Class that manages the lifetime of a OpenGL program
class Program
{
    GLuint const mHandle = 0;

public:
    Program() = delete;
    Program(const Program&) = delete;
    Program(Program&&) = delete;
    Program& operator=(const Program&) = delete;
    Program& operator=(Program&&) = delete;
    ~Program();

    //! @brief Constructor that takes as many shaders as desired and links the program
    //! @tparam TShader: Varying number of shaders
    //! @param shaderList: List of shaders
    //! @remark Even though the template should be callable with any input type, only the Shader class will compile.
    //! Didn't find a better way to use a parameter pack.
    template <typename... TShader>
    Program(const TShader&... shaderList)
        : mHandle(glCreateProgram())
    {
        Initialize({std::forward<const TShader&>(shaderList)...});
    }


    //! @brief Constructor that takes as many shaders as desired and links the program (initializer version)
    //! @param shaderList: List of shaders
    Program(std::initializer_list<std::reference_wrapper<const Shader>> shaderList);

    //! @brief Gets the programs handle
    //! @return Program handle
    GLuint GetHandle() const
    {
        return mHandle;
    }

    //! @brief Queries the location of a uniform with the given name
    //! @param uniformName: Name of the uniform
    //! @return Handle of the uniform
    GLuint QueryUniformLocation(const std::string& uniformName) const;


    //! @brief Sets the value of an uniform
    //! @tparam TTypeEnum: GL type enum
    //! @tparam TValue: Value type
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    template <typename TValue>
    void SetUniform(GLint uniformLocation, TValue value) const;


    //! @brief Sets the values of an uniform array
    //! @tparam TTypeEnum: GL type enum
    //! @tparam TValue: Value type
    //! @param uniformLocation: location of the first uniform array member that should be set
    //! @param values: Vector with values
    template <typename TValue>
    void SetUniformArray(GLint uniformLocation, const TValue* const values, U32 size) const;

    //! @brief Sets this program as active program
    void Use() const;

private:
    //! @brief Checks if the included shaders can actually be linked into a program
    void CheckShaders(std::initializer_list<std::reference_wrapper<const GDL::OpenGL::Shader>> shaderList) const;

    //! @brief Checks if the program links without errors
    void CheckLinkStatus() const;

    //! @brief Checks if any errors have occured
    void CheckForErrors() const;

    //! @brief Links all the passed shaders to the program
    //! @param shaderList: Shaders that should be linked
    void Initialize(std::initializer_list<std::reference_wrapper<const Shader>> shaderList);
};
}
}
