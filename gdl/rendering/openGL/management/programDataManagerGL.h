#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/rendering/openGL/core/programGL.h"
#include "gdl/rendering/openGL/management/programInput.h"
#include "gdl/rendering/openGL/management/uniform.h"
#include "gdl/rendering/openGL/management/uniformBlock.h"

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
    const ProgramGL& mProgram;

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
    ProgramDataManager(const ProgramGL& program)
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
    //! @tparam TTypeEnum: GL type enum
    //! @tparam TValue: Value type
    //! @param uniformLocation: Location of the uniform
    //! @param value: New value
    template <GLuint TTypeEnum, typename TValue>
    void SetUniform(GLuint uniformLocation, TValue value) const
    {
        assert(GetUniformByLocation(uniformLocation).GetType() == TTypeEnum);
        mProgram.SetUniform<TTypeEnum>(uniformLocation, value);
    }

    //! @brief Sets the value of an uniform of the managed program
    //! @tparam TTypeEnum: GL type enum
    //! @tparam TValue: Value type
    //! @param uniformName: Name of the uniform
    //! @param value: New value
    template <GLuint TTypeEnum, typename TValue>
    void SetUniform(std::string uniformName, TValue value) const
    {
        SetUniform<TTypeEnum>(GetUniform(uniformName).GetLocation(), value);
    }


    //! @brief Sets the values of an uniform array of the managed program
    //! @tparam TTypeEnum: GL type enum
    //! @tparam TValue: Value type
    //! @param uniformLocation: location of the first uniform array member that should be set
    //! @param values: Vector with values
    template <GLuint TTypeEnum, typename TValue>
    void SetUniformArray(GLuint uniformLocation, const TValue* const values, U32 size) const
    {
        assert(GetUniformByLocation(uniformLocation).GetType() == TTypeEnum);
        assert(GetUniformByLocation(uniformLocation).GetArraySize() >= size);
        mProgram.SetUniformArray<TTypeEnum>(uniformLocation, values, size);
    }

    //! @brief Sets the values of an uniform array of the managed program
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

    //! @brief Gets a uniform by its location
    //! @param handle: The uniforms location
    //! @return Uniform
    const Uniform& GetUniformByLocation(GLint location) const;
};
}
}
