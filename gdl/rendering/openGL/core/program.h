#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/mat4.h"

#include <GL/glew.h>
#include <functional>

namespace GDL::OpenGL
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
    //! @tparam _shader: Varying number of shaders
    //! @param shaderList: List of shaders
    //! @remark Even though the template should be callable with any input type, only the Shader class will compile.
    //! Didn't find a better way to use a parameter pack.
    template <typename... _shader>
    Program(const _shader&... shaderList);

    //! @brief Constructor that takes as many shaders as desired and links the program (initializer version)
    //! @param shaderList: List of shaders
    Program(std::initializer_list<std::reference_wrapper<const Shader>> shaderList);

    //! @brief Gets the programs handle
    //! @return Program handle
    GLuint GetHandle() const;

    //! @brief Queries the location of a uniform with the given name
    //! @param uniformName: Name of the uniform
    //! @return Handle of the uniform
    GLint QueryUniformLocation(const char* uniformName) const;

    //! @brief Sets the value of a uniform
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    void SetUniform(GLint uniformLocation, F32 value) const;

    //! @brief Sets the value of a uniform
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    void SetUniform(GLint uniformLocation, const Mat4f& value) const;

    //! @brief Sets the values of a uniform array
    //! @param uniformLocation: location of the first uniform array member that should be set
    //! @param values: Vector with values
    void SetUniformArray(GLint uniformLocation, const F32* const values, U32 size) const;

    //! @brief Sets this program as active program
    void Use() const;

private:
    //! @brief Checks if the program links without errors
    void CheckLinkStatus() const;

    //! @brief Checks if any errors have occured
    void CheckForErrors() const;

    //! @brief Links all the passed shaders to the program
    //! @param shaderList: Shaders that should be linked
    void Initialize(std::initializer_list<std::reference_wrapper<const Shader>> shaderList);
};


// Template definitions ---------------------------------------------------------------------------

template <typename... _shader>
Program::Program(const _shader&... shaderList)
    : mHandle(glCreateProgram())
{
    Initialize({std::forward<const _shader&>(shaderList)...});
}


} // namespace GDL::OpenGL
