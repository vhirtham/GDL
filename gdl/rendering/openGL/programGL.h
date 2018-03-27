#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"
#include "gdl/rendering/openGL/programInput.h"
#include "gdl/rendering/openGL/shaderGL.h"
#include "gdl/rendering/openGL/uniform.h"
#include "gdl/rendering/openGL/uniformBlock.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace GDL
{
class ProgramGL
{
    GLuint const mHandle = 0;
    std::map<const std::string, const Uniform> mUniforms;
    std::map<const std::string, UniformBlock> mUniformBlocks;
    std::map<const std::string, const ProgramInput> mInputs;

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
    ProgramGL(const TShader&... shaderList)
        : mHandle(glCreateProgram())
    {
        Initialize({std::forward<const TShader&>(shaderList)...});
    }


    //! @brief Constructor that takes as many shaders as desired and links the program (initializer version)
    //! @param shaderList: List of shaders
    ProgramGL(std::initializer_list<std::reference_wrapper<const ShaderGL>> shaderList);

    //! @brief Gets the programs handle
    //! @return Program handle
    GLuint GetHandle() const
    {
        return mHandle;
    }

    //! @brief Gets the specified program input
    //! @param inputName: Name of the input
    //! @return Specified input
    ProgramInput GetInput(std::string inputName) const;

    //! @brief Gets the number of inputs
    //! @return Number of inputs
    U32 GetNumInputs() const
    {
        return mInputs.size();
    }

    //! @brief Gets the number of uniforms
    //! @return Number of uniforms
    U32 GetNumUniforms() const
    {
        return mUniforms.size();
    }

    //! @brief Gets the an uniform
    //! @param uniformName: Name of the uniform
    //! @return The uniform
    Uniform GetUniform(std::string uniformName) const;



    //! @brief Sets the value of an uniform
    //! @tparam TTypeEnum: GL type enum
    //! @tparam TValue: Value type
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    template <GLuint TTypeEnum, typename TValue>
    void SetUniform(GLuint uniformLocation, TValue value) const;

    //! @brief Sets the value of an uniform
    //! @tparam TTypeEnum: GL type enum
    //! @tparam TValue: Value type
    //! @param uniformName: Name of the uniform
    //! @param value: New value
    template <GLuint TTypeEnum, typename TValue>
    void SetUniform(std::string uniformName, TValue value) const
    {
        SetUniform<TTypeEnum>(GetUniform(uniformName).GetLocation(), value);
    }


    //! @brief Sets the values of an uniform array
    //! @tparam TTypeEnum: GL type enum
    //! @tparam TValue: Value type
    //! @param uniformLocation: location of the first uniform array member that should be set
    //! @param values: Vector with values
    template <GLuint TTypeEnum, typename TValue>
    void SetUniformArray(GLuint uniformLocation, const TValue* const values, U32 size) const;

    //! @brief Sets the values of an uniform array
    //! @tparam TTypeEnum: GL type enum
    //! @tparam TValue: Value type
    //! @param uniformName: Name of the uniform
    //! @param values: Vector with values
    template <GLuint TTypeEnum, typename TValue>
    void SetUniformArray(std::string uniformName, const TValue* const values, U32 size) const
    {
        SetUniformArray<TTypeEnum>(GetUniform(uniformName).GetLocation(), values, size);
    }


private:
    //! @brief Checks if the program links without errors
    void CheckLinkStatus() const;

    //! @brief Gathers program information (uniforms, subroutines, inputs etc.)
    void GatherProgramData();

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
                                                                      std::array<GLenum, TNumProps> properties) const;

    //! @brief Gets the name of a resource
    //! @param eResourceType: Enum that speciefies the resource type
    //! @param handle: Resource handle
    //! @param buffersize: Length of the buffer that gets the name (must be queried)
    //! @return Name of the resource
    std::string GetResourceName(GLenum eResourceType, GLuint index, GLint bufferSize) const;

#ifndef NDEBUG
    //! @brief Gets a uniform by its location
    //! @param handle: The uniforms location
    //! @return Uniform
    //! @remark DEBUG only!
    const Uniform& GetUniformByLocation(GLint location) const;
#endif
};
}
