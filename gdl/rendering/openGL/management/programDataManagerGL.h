#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/rendering/openGL/core/programGL.h"
#include "gdl/rendering/openGL/management/programInputGL.h"
#include "gdl/rendering/openGL/management/uniformGL.h"
#include "gdl/rendering/openGL/management/uniformBlockGL.h"

#include <cassert>
#include <map>
#include <string>
#include <vector>

namespace GDL
{
namespace OpenGL
{

//! @brief Gathers information about a programs data(uniforms, inputs etc.) and provides additional functionality to
//! modify it.
class ProgramDataManager
{
    const Program& mProgram;

    std::map<const std::string, const Uniform> mUniforms;
    std::map<const std::string, UniformBlock> mUniformBlocks;
    std::map<const std::string, const ProgramInput> mInputs;

public:
    ProgramDataManager() = delete;
    ProgramDataManager(const ProgramDataManager&) = delete;
    ProgramDataManager(ProgramDataManager&&) = delete;
    ProgramDataManager& operator=(const ProgramDataManager&) = delete;
    ProgramDataManager& operator=(ProgramDataManager&&) = delete;
    ~ProgramDataManager() = default;

    //! @brief Constructor that gathers all informations about the passed program
    //! @param program: OpenGL program that should be managed
    ProgramDataManager(const Program& program)
        : mProgram(program)
    {
        GatherProgramData();
    }

    //! @brief Gets the handle of the managed program
    //! @return Handle of the manged Program
    GLuint GetHandle() const
    {
        return mProgram.GetHandle();
    }

    //! @brief Gets the specified program input
    //! @param inputName: Name of the input of the managed program
    //! @return Specified input
    const ProgramInput& GetInput(std::string inputName) const;

    //! @brief Gets the number of inputs of the managed program
    //! @return Number of inputs of the managed program
    U32 GetNumInputs() const
    {
        return mInputs.size();
    }


    //! @brief Gets an uniform of the managed program
    //! @param uniformName: Name of the uniform
    //! @return The specified uniform
    const Uniform& GetUniform(std::string uniformName) const;

    //! @brief Gets the number of uniforms of the managed program
    //! @return Number of uniforms of the managed program
    U32 GetNumUniforms() const
    {
        return mUniforms.size();
    }

    //! @brief Gets a uniform block of the managed program
    //! @param uniformBlockName: Name of the uniform block
    //! @return The uniform block
    const UniformBlock& GetUniformBlock(std::string uniformBlockName) const;

    //! @brief Gets the number of uniforms blocks of the managed program
    //! @return Number of uniform blocks of the managed program
    U32 GetNumUniformBlocks() const
    {
        return mUniformBlocks.size();
    }

    //! @brief Sets the value of an uniform of the managed program
    //! @tparam _typeEnum: GL type enum
    //! @tparam _value: Value type
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    template <GLuint _typeEnum, typename _value>
    void SetUniform(GLint uniformLocation, _value value) const
    {
        assert(GetUniformByLocation(uniformLocation).GetType() == _typeEnum);
        mProgram.SetUniform(uniformLocation, value);
    }

    //! @brief Sets the value of an uniform of the managed program
    //! @tparam _typeEnum: GL type enum
    //! @tparam _value: Value type
    //! @param uniformName: Name of the uniform
    //! @param value: New value
    template <GLuint _typeEnum, typename _value>
    void SetUniform(std::string uniformName, _value value) const
    {
        SetUniform<_typeEnum>(GetUniform(uniformName).GetLocation(), value);
    }


    //! @brief Sets the values of an uniform array of the managed program
    //! @tparam _typeEnum: GL type enum
    //! @tparam _value: Value type
    //! @param uniformLocation: location of the first uniform array member that should be set
    //! @param values: Vector with values
    template <GLuint _typeEnum, typename _value>
    void SetUniformArray(GLint uniformLocation, const _value* const values, U32 size) const
    {
        assert(GetUniformByLocation(uniformLocation).GetType() == _typeEnum);
        assert(GetUniformByLocation(uniformLocation).GetArraySize() >= size);
        mProgram.SetUniformArray(uniformLocation, values, size);
    }

    //! @brief Sets the values of an uniform array of the managed program
    //! @tparam _typeEnum: GL type enum
    //! @tparam _value: Value type
    //! @param uniformName: Name of the uniform
    //! @param values: Vector with values
    template <GLuint _typeEnum, typename _value>
    void SetUniformArray(std::string uniformName, const _value* const values, U32 size) const
    {
        SetUniformArray<_typeEnum>(GetUniform(uniformName).GetLocation(), values, size);
    }

    //! @brief Sets this program as active program
    void Use() const;


private:
    //! @brief Gathers program information (uniforms, subroutines, inputs etc.)
    void GatherProgramData();

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

    //! @brief Finds all variables of one  of the programs uniform blocks and stores them.
    //! @param uniformBlock: The uniform block which varibles should be found
    //! @param numVariables: Number of variables (must be queried before)
    void FindUniformBlockVariables(UniformBlock& uniformBlock, GLuint numVariables);

    //! @brief Finds the specified properties of all instances of the given resource type
    //! @param eResourceType: Enum that speciefies the resource type
    //! @param properties: Proberties that should be determined
    //! @tparam _numProps: Number of properties that should be determined
    template <U32 _numProps>
    std::vector<std::array<GLint, _numProps>> FindProgramResourceData(GLenum eResourceType,
                                                                      std::array<GLenum, _numProps> properties) const;

    //! @brief Gets the name of a resource
    //! @param eResourceType: Enum that speciefies the resource type
    //! @param handle: Resource handle
    //! @param buffersize: Length of the buffer that gets the name (must be queried)
    //! @return Name of the resource
    std::string GetResourceName(GLenum eResourceType, GLuint index, GLint bufferSize) const;

    //! @brief Gets a uniform by its location
    //! @param handle: The uniforms location
    //! @return Uniform
    const Uniform& GetUniformByLocation(GLint location) const;
};
} // namespace OpenGL
} // namespace GDL
