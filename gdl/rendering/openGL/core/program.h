#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/mat4.h"

#include <GL/glew.h>
#include <array>
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

    //! @brief Queries the binding point of a uniform block
    //! @param uniformBlockIndex: Index of the uniform block
    //! @return Binding point of the uniform block
    GLuint QueryUniformBlockBinding(GLuint uniformBlockIndex) const;

    //! @brief Queries the index of a uniform block
    //! @param uniformBlockName: Name of the uniform block
    //! @return Index of the uniform block
    GLuint QueryUniformBlockIndex(const char* uniformBlockName) const;

    //! @brief Queries the size of a uniform block
    //! @param uniformBlockIndex: Index of the uniform block
    //! @return Size of the uniform block
    GLint QueryUniformBlockSize(GLuint uniformBlockIndex) const;

    //! @brief Queries the indeces of an arbitrary number of uniforms of a uniform block
    //! @tparam _numUniforms: Number of uniform indices that should be queried
    //! @param: uniformNames: Names of the uniforms
    //! @return: Indices of the uniforms
    template <U32 _numUniforms>
    std::array<GLuint, _numUniforms>
    QueryUniformBlockUniformIndices(const std::array<const char*, _numUniforms>& uniformNames) const;

    //! @brief Queries the offsets of an arbitrary number of uniforms of a uniform block
    //! @tparam _numUniforms: Number of uniform offsets that should be queried
    //! @param: uniformIndices: Indices of the uniforms
    //! @return: Offsets of the uniforms
    template <U32 _numUniforms>
    std::array<GLint, _numUniforms>
    QueryUniformBlockUniformOffsets(const std::array<GLuint, _numUniforms>& uniformIndices) const;

    //! @brief Queries the offsets of an arbitrary number of uniforms of a uniform block
    //! @tparam _numUniforms: Number of uniform offsets that should be queried
    //! @param: uniformNames: Names of the uniforms
    //! @return: Offsets of the uniforms
    template <U32 _numUniforms>
    std::array<GLint, _numUniforms>
    QueryUniformBlockUniformOffsets(const std::array<const char*, _numUniforms>& uniformNames) const;

    //! @brief Queries the location of a uniform with the given name
    //! @param uniformName: Name of the uniform
    //! @return Location of the uniform
    GLint QueryUniformLocation(const char* uniformName) const;

    //! @brief Sets the value of a uniform float
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    void SetUniform(GLint uniformLocation, F32 value);

    //! @brief Sets the value of a uniform int
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    void SetUniform(GLint uniformLocation, I32 value);

    //! @brief Sets the value of a uniform unsigned int
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    void SetUniform(GLint uniformLocation, U32 value);

    //! @brief Sets the value of a uniform vec2
    //! @param uniformLocation: Location of the uniform
    //! @param values: New values
    void SetUniform(GLint uniformLocation, const std::array<F32, 2>& values);

    //! @brief Sets the value of a uniform mat4
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    void SetUniform(GLint uniformLocation, const Mat4f& values);

    //! @brief Sets the texture unit of a uniform sampler
    //! @param uniformName: Name of the uniform sampler
    //! @param textureUnit: Texture unit
    void SetUniformSamplerTextureUnit(const char* uniformName, U32 textureUnit);

    //! @brief Sets the values of a uniform array
    //! @param uniformLocation: location of the first uniform array member that should be set
    //! @param values: Vector with values
    void SetUniformArray(GLint uniformLocation, const F32* const values, U32 size);

    //! @brief Sets the binding point of a uniform block
    //! @param uniformBlockIndex: Index of the uniform block
    //! @param bindingPoint: Target binding point
    void SetUniformBlockBinding(GLuint uniformBlockIndex, GLuint bindingPoint);

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



template <U32 _numUniforms>
std::array<GLuint, _numUniforms>
Program::QueryUniformBlockUniformIndices(const std::array<const char*, _numUniforms>& uniformNames) const
{
    std::array<GLuint, _numUniforms> indices;
    glGetUniformIndices(mHandle, _numUniforms, uniformNames.data(), indices.data());
    return indices;
}



template <U32 _numUniforms>
std::array<GLint, _numUniforms>
Program::QueryUniformBlockUniformOffsets(const std::array<GLuint, _numUniforms>& uniformIndices) const
{
    std::array<GLint, _numUniforms> offsets;
    glGetActiveUniformsiv(mHandle, _numUniforms, uniformIndices.data(), GL_UNIFORM_OFFSET, offsets.data());
    return offsets;
}



template <U32 _numUniforms>
std::array<GLint, _numUniforms>
Program::QueryUniformBlockUniformOffsets(const std::array<const char*, _numUniforms>& uniformNames) const
{
    return QueryUniformBlockUniformOffsets<_numUniforms>(QueryUniformBlockUniformIndices<_numUniforms>(uniformNames));
}

} // namespace GDL::OpenGL
