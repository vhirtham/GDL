#include "programDataManagerGL.h"


#include "gdl/base/exception.h"

#include <memory>

const GDL::OpenGL::ProgramInput& GDL::OpenGL::ProgramDataManager::GetInput(std::string inputName) const
{
    auto iterator = mInputs.find(inputName);
    if (iterator == mInputs.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find input \"" + inputName + "\". GLSL compiler optimization might be the reason.");
    return iterator->second;
}

const GDL::OpenGL::Uniform& GDL::OpenGL::ProgramDataManager::GetUniform(std::string uniformName) const
{
    auto iterator = mUniforms.find(uniformName);
    if (iterator == mUniforms.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find uniform \"" + uniformName +
                                "\". GLSL compiler optimization might be the reason.");
    return iterator->second;
}

const GDL::OpenGL::UniformBlock& GDL::OpenGL::ProgramDataManager::GetUniformBlock(std::string uniformBlockName) const
{
    auto iterator = mUniformBlocks.find(uniformBlockName);
    if (iterator == mUniformBlocks.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find uniform block \"" + uniformBlockName +
                                "\". GLSL compiler optimization might be the reason.");
    return iterator->second;
}

void GDL::OpenGL::ProgramDataManager::Use() const
{
    mProgram.Use();
}

void GDL::OpenGL::ProgramDataManager::GatherProgramData()
{
    FindInputs();
    FindUniforms();
    FindUniformBlocks();
}

void GDL::OpenGL::ProgramDataManager::FindInputs()
{
    auto inputData = FindProgramResourceData<3>(GL_PROGRAM_INPUT, {{GL_LOCATION, GL_NAME_LENGTH, GL_TYPE}});
    for (U32 i = 0; i < inputData.size(); ++i)
        mInputs.emplace(GetResourceName(GL_PROGRAM_INPUT, i, inputData[i][1]),
                        ProgramInput(inputData[i][0], inputData[i][2]));
}

void GDL::OpenGL::ProgramDataManager::FindUniforms()
{
    auto data = FindProgramResourceData<5>(GL_UNIFORM,
                                           {{GL_LOCATION, GL_NAME_LENGTH, GL_TYPE, GL_ARRAY_SIZE, GL_BLOCK_INDEX}});
    for (U32 i = 0; i < data.size(); ++i)
    {
        if (data[i][4] != -1) // if uniform is member of a Uniform Block (handled seperately)
            continue;

        auto uniformIt = mUniforms
                                 .emplace(GetResourceName(GL_UNIFORM, i, data[i][1]),
                                          Uniform(data[i][0], data[i][2], data[i][3]))
                                 .first;

        FindUniformArrayMembers(uniformIt->first, uniformIt->second);
    }
}

void GDL::OpenGL::ProgramDataManager::FindUniformArrayMembers(const std::string& firstElementName,
                                                              const Uniform& firstElement)
{
    if (firstElement.GetArraySize() > 1)
    {
        std::string arrayName = firstElementName;
        assert(arrayName.find("[0]", arrayName.length() - 3) == arrayName.length() - 3);
        arrayName.erase(arrayName.length() - 3, 3);

        for (GLuint j = 1; j < firstElement.GetArraySize(); ++j)
        {
            std::string arrayElementName = arrayName + "[" + std::to_string(j) + "]";
            GLint arrayElementHandle = glGetUniformLocation(mProgram.GetHandle(), arrayElementName.c_str());

            if (arrayElementHandle < 0)
                throw Exception(__PRETTY_FUNCTION__,
                                "Could not find location of array element \"" + arrayElementName +
                                        "\"! This error might be caused by GLSL compiler optimizaion");

            mUniforms.emplace(arrayElementName,
                              Uniform(arrayElementHandle, firstElement.GetType(), firstElement.GetArraySize() - j));
        }
    }
}

void GDL::OpenGL::ProgramDataManager::FindUniformBlocks()
{
    auto data = FindProgramResourceData<4>(
            GL_UNIFORM_BLOCK, {{GL_NAME_LENGTH, GL_BUFFER_BINDING, GL_NUM_ACTIVE_VARIABLES, GL_BUFFER_DATA_SIZE}});
    for (GLuint i = 0; i < data.size(); ++i)
    {
        std::string uniformBlockName = GetResourceName(GL_UNIFORM_BLOCK, i, data[i][0]);
        assert(glGetUniformBlockIndex(mProgram.GetHandle(), uniformBlockName.c_str()) == i);

        UniformBlock& uniformBlock =
                mUniformBlocks.emplace(uniformBlockName, UniformBlock(i, data[i][1], data[i][3])).first->second;

        FindUniformBlockVariables(uniformBlock, data[i][2]);
    }
}

void GDL::OpenGL::ProgramDataManager::FindUniformBlockVariables(UniformBlock& uniformBlock, GLuint numVariables)
{
    std::vector<GLint> indices(numVariables);
    std::vector<GLint> nameLengths(numVariables);
    std::vector<GLint> offsets(numVariables);
    std::vector<GLint> arraySizes(numVariables);
    std::vector<GLint> types(numVariables);

    const GLuint handle = mProgram.GetHandle();

    glGetActiveUniformBlockiv(handle, uniformBlock.GetIndex(), GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices.data());
    std::vector<GLuint> indicesUnsigned(indices.begin(), indices.end());
    glGetActiveUniformsiv(handle, numVariables, indicesUnsigned.data(), GL_UNIFORM_NAME_LENGTH, nameLengths.data());
    glGetActiveUniformsiv(handle, numVariables, indicesUnsigned.data(), GL_UNIFORM_OFFSET, offsets.data());
    glGetActiveUniformsiv(handle, numVariables, indicesUnsigned.data(), GL_UNIFORM_SIZE, arraySizes.data());
    glGetActiveUniformsiv(handle, numVariables, indicesUnsigned.data(), GL_UNIFORM_TYPE, types.data());

    for (GLuint i = 0; i < numVariables; ++i)
    {
        uniformBlock.mVariables.emplace(GetResourceName(GL_UNIFORM, indicesUnsigned[i], nameLengths[i]),
                                        UniformBlockVariable(indicesUnsigned[i], offsets[i], types[i], arraySizes[i]));
    }
}

std::string GDL::OpenGL::ProgramDataManager::GetResourceName(GLenum eResourceType, GLuint index, GLint bufferSize) const
{
    std::unique_ptr<GLchar[]> resourceName = std::make_unique<GLchar[]>(bufferSize);
    glGetProgramResourceName(mProgram.GetHandle(), eResourceType, index, bufferSize, nullptr, resourceName.get());
    std::string nameString(resourceName.get());
    return nameString;
}

const GDL::OpenGL::Uniform& GDL::OpenGL::ProgramDataManager::GetUniformByLocation(GLint location) const
{
    for (const auto& uniform : mUniforms)
        if (uniform.second.GetLocation() == location)
            return uniform.second;

    throw Exception(__PRETTY_FUNCTION__, "Uniform not found!");
}


template <GDL::U32 TNumProps>
std::vector<std::array<GLint, TNumProps>>
GDL::OpenGL::ProgramDataManager::FindProgramResourceData(GLenum eResourceType,
                                                         std::array<GLenum, TNumProps> properties) const
{
    GLint numResources = 0;
    glGetProgramInterfaceiv(mProgram.GetHandle(), eResourceType, GL_ACTIVE_RESOURCES, &numResources);
    std::vector<std::array<GLint, TNumProps>> results(numResources);
    for (GLint i = 0; i < numResources; ++i)
    {
        glGetProgramResourceiv(mProgram.GetHandle(), eResourceType, i, TNumProps, properties.data(), TNumProps, nullptr,
                               results[i].data());
    }
    return results;
}
