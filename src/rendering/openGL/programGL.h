#pragma once

#include "GDLTypedefs.h"
#include "base/Exception.h"
#include "rendering/openGL/shaderGL.h"
#include "rendering/openGL/Uniform.h"

#include <functional>
#include <map>
#include <vector>

namespace GDL
{
class ProgramGL
{
    GLuint const mHandle = 0;
    std::map<const std::string, const Uniform> mUniforms;
    std::map<const std::string, const GLuint> mProgramInputs;

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
    ProgramGL(std::initializer_list<std::reference_wrapper<const ShaderGL>> shaderList);

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

    //! @brief Sets the values of an uniform array
    //! @param uniformHandle: handle of the first uniform array member that should be set
    //! @param values: Vector with values
    template <typename TScalar>
    void SetUniformScalarArray(GLuint uniformHandle, std::vector<TScalar> values);

    //! @brief Sets the values of an uniform array
    //! @param uniformHandle: Name of the first uniform array member that should be set (including "[i]")
    //! @param values: Vector with values
    template <typename TScalar>
    void SetUniformScalarArray(std::string uniformName, std::vector<TScalar> values);

private:
    //! @brief Checks if the program links without errors
    void CheckLinkStatus();

    //! @brief Links all the passed shaders to the program
    //! @param shaderList: Shaders that should be linked
    void Initialize(std::initializer_list<std::reference_wrapper<const ShaderGL>> shaderList);

    //! @brief Finds all the programs inputs and stores their names and handles.
    void FindInputs();

    //! @brief Finds all the programs uniforms and stores their names and handles.
    void FindUniforms();

    //! @brief Finds the additional members locations of a uniform array and stores their names and handles.
    //! @param firstElementName: Name of the first element -> "arrayName[0]"
    //! @param firstElement: First elements uniform data
    void FindUniformArrayMembers(const std::string& firstElementName, const Uniform& firstElement);

    //! @brief Finds all the programs uniform blocks and stores their names and handles.
    void FindUniformBlocks();

    //! @brief Finds the specified properties of all instances of the given resource type
    //! @param eResourceType: Enum that speciefies the resource type
    //! @param properties: Proberties that should be determined
    //! @tparam TNumProps: Number of properties that should be determined
    template <U32 TNumProps>
    std::vector<std::array<GLint, TNumProps>> FindProgramResourceData(GLenum eResourceType,
                                                                      std::array<GLenum, TNumProps> properties);

    //! @brief Gets the name of a resource
    //! @param eResourceType: Enum that speciefies the resource type
    //! @param handle: Resource handle
    //! @param buffersize: Length of the buffer that gets the name (must be queried)
    //! @return Name of the resource
    std::string GetResourceName(GLenum eResourceType, GLuint index, GLint bufferSize);

#ifndef NDEBUG
    //! @brief Gets a uniform by its handle
    //! @param handle: The uniforms handle
    //! @return Uniform
    //! @remark DEBUG only!
    const Uniform& GetUniformByHandle(GLuint handle);
#endif
};
}



template <typename TScalar>
void GDL::ProgramGL::SetUniformScalar(std::string uniformName, TScalar value)
{
    SetUniformScalar(GetUniformHandle(uniformName), value);
}

template <typename TScalar>
void GDL::ProgramGL::SetUniformScalarArray(std::string uniformName, std::vector<TScalar> values)
{
    SetUniformScalarArray(GetUniformHandle(uniformName), values);
}

template <typename... TShader>
GDL::ProgramGL::ProgramGL(const TShader&... shaderList)
    : mHandle(glCreateProgram())
{
    Initialize({std::forward<const TShader&>(shaderList)...});
}
