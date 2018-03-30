#include "programGL.h"

#include "gdl/base/Exception.h"

#include <cassert>
#include <memory>
#include <set>

GDL::ProgramGL::~ProgramGL()
{
    glDeleteProgram(mHandle);
}



GDL::ProgramGL::ProgramGL(std::initializer_list<std::reference_wrapper<const ShaderGL>> shaderList)
    : mHandle(glCreateProgram())
{
    Initialize(shaderList);
}

const GDL::ProgramInput& GDL::ProgramGL::GetInput(std::string inputName) const
{
    auto iterator = mInputs.find(inputName);
    if (iterator == mInputs.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find input \"" + inputName + "\". GLSL compiler optimization might be the reason.");
    return iterator->second;
}

const GDL::Uniform& GDL::ProgramGL::GetUniform(std::string uniformName) const
{
    auto iterator = mUniforms.find(uniformName);
    if (iterator == mUniforms.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find uniform \"" + uniformName +
                                "\". GLSL compiler optimization might be the reason.");
    return iterator->second;
}

const GDL::UniformBlock& GDL::ProgramGL::GetUniformBlock(std::string uniformBlockName) const
{
    auto iterator = mUniformBlocks.find(uniformBlockName);
    if (iterator == mUniformBlocks.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find uniform block \"" + uniformBlockName +
                                "\". GLSL compiler optimization might be the reason.");
    return iterator->second;
}



void GDL::ProgramGL::CheckLinkStatus() const
{
    GLint status;
    glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::unique_ptr<GLchar[]> infoLog = std::make_unique<GLchar[]>(infoLogLength);
        glGetProgramInfoLog(mHandle, infoLogLength, nullptr, infoLog.get());

        std::string errorMessage(infoLog.get());
        throw Exception(__PRETTY_FUNCTION__, "Failed to compile shader program \n\nLog Message:\n" + errorMessage);
    }
}

void GDL::ProgramGL::GatherProgramData()
{
    FindInputs();
    FindUniforms();
    FindUniformBlocks();
}

void GDL::ProgramGL::Initialize(std::initializer_list<std::reference_wrapper<const GDL::ShaderGL>> shaderList)
{
    std::set<GLenum> attachedShaderTypes;
    for (auto& shader : shaderList)
    {
        if (attachedShaderTypes.find(shader.get().GetType()) != attachedShaderTypes.end())
            throw Exception(__PRETTY_FUNCTION__,
                            "Multiple shaders of the same type in one program are not "
                            "supported.\n You tried to attach multiple shaders of type: " +
                                    ShaderGL::GetShaderTypeString(shader.get().GetType()));
        glAttachShader(mHandle, shader.get().GetHandle());
        attachedShaderTypes.emplace(shader.get().GetType());
    }

    if (attachedShaderTypes.find(GL_VERTEX_SHADER) == attachedShaderTypes.end() ||
        attachedShaderTypes.find(GL_FRAGMENT_SHADER) == attachedShaderTypes.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "You have to provide a vertex shader and a fragment shader for each program!");

    glLinkProgram(mHandle);

    for (auto& shader : shaderList)
        glDetachShader(mHandle, shader.get().GetHandle());


    CheckLinkStatus();
    GatherProgramData();

    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR)
        throw Exception(__PRETTY_FUNCTION__,
                        "Could not create program:\n" +
                                std::string(reinterpret_cast<const char*>(gluErrorString(errorCode))));
}



void GDL::ProgramGL::FindInputs()
{
    auto inputData = FindProgramResourceData<3>(GL_PROGRAM_INPUT, {{GL_LOCATION, GL_NAME_LENGTH, GL_TYPE}});
    for (U32 i = 0; i < inputData.size(); ++i)
        mInputs.emplace(GetResourceName(GL_PROGRAM_INPUT, i, inputData[i][1]),
                        ProgramInput(inputData[i][0], inputData[i][2]));
}



void GDL::ProgramGL::FindUniforms()
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

void GDL::ProgramGL::FindUniformArrayMembers(const std::string& firstElementName, const Uniform& firstElement)
{
    if (firstElement.GetArraySize() > 1)
    {
        std::string arrayName = firstElementName;
        assert(arrayName.find("[0]", arrayName.length() - 3) == arrayName.length() - 3);
        arrayName.erase(arrayName.length() - 3, 3);

        for (GLuint j = 1; j < firstElement.GetArraySize(); ++j)
        {
            std::string arrayElementName = arrayName + "[" + std::to_string(j) + "]";
            GLint arrayElementHandle = glGetUniformLocation(mHandle, arrayElementName.c_str());

            if (arrayElementHandle < 0)
                throw Exception(__PRETTY_FUNCTION__,
                                "Could not find location of array element \"" + arrayElementName +
                                        "\"! This error might be caused by GLSL compiler optimizaion");

            mUniforms.emplace(arrayElementName,
                              Uniform(arrayElementHandle, firstElement.GetType(), firstElement.GetArraySize() - j));
        }
    }
}

void GDL::ProgramGL::FindUniformBlocks()
{
    auto data = FindProgramResourceData<4>(
            GL_UNIFORM_BLOCK, {{GL_NAME_LENGTH, GL_BUFFER_BINDING, GL_NUM_ACTIVE_VARIABLES, GL_BUFFER_DATA_SIZE}});
    for (GLuint i = 0; i < data.size(); ++i)
    {
        std::string uniformBlockName = GetResourceName(GL_UNIFORM_BLOCK, i, data[i][0]);
        assert(glGetUniformBlockIndex(mHandle, uniformBlockName.c_str()) == i);

        UniformBlock& uniformBlock =
                mUniformBlocks.emplace(uniformBlockName, UniformBlock(i, data[i][1], data[i][3])).first->second;

        FindUniformBlockVariables(uniformBlock, data[i][2]);
    }
}

void GDL::ProgramGL::FindUniformBlockVariables(GDL::UniformBlock& uniformBlock, GLuint numVariables)
{
    std::vector<GLint> indices(numVariables);
    std::vector<GLint> nameLengths(numVariables);
    std::vector<GLint> offsets(numVariables);
    std::vector<GLint> arraySizes(numVariables);
    std::vector<GLint> types(numVariables);

    glGetActiveUniformBlockiv(mHandle, uniformBlock.GetIndex(), GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                              indices.data());
    std::vector<GLuint> indicesUnsigned(indices.begin(), indices.end());
    glGetActiveUniformsiv(mHandle, numVariables, indicesUnsigned.data(), GL_UNIFORM_NAME_LENGTH, nameLengths.data());
    glGetActiveUniformsiv(mHandle, numVariables, indicesUnsigned.data(), GL_UNIFORM_OFFSET, offsets.data());
    glGetActiveUniformsiv(mHandle, numVariables, indicesUnsigned.data(), GL_UNIFORM_SIZE, arraySizes.data());
    glGetActiveUniformsiv(mHandle, numVariables, indicesUnsigned.data(), GL_UNIFORM_TYPE, types.data());

    for (GLuint i = 0; i < numVariables; ++i)
    {
        uniformBlock.mVariables.emplace(GetResourceName(GL_UNIFORM, indicesUnsigned[i], nameLengths[i]),
                                        UniformBlockVariable(indicesUnsigned[i], offsets[i], types[i], arraySizes[i]));
    }
}



std::string GDL::ProgramGL::GetResourceName(GLenum eResourceType, GLuint index, GLint bufferSize) const
{
    std::unique_ptr<GLchar[]> resourceName = std::make_unique<GLchar[]>(bufferSize);
    glGetProgramResourceName(mHandle, eResourceType, index, bufferSize, nullptr, resourceName.get());
    std::string nameString(resourceName.get());
    return nameString;
}

#ifndef NDEBUG
const GDL::Uniform& GDL::ProgramGL::GetUniformByLocation(GLint location) const
{
    for (const auto& uniform : mUniforms)
        if (uniform.second.GetLocation() == location)
            return uniform.second;

    throw Exception(__PRETTY_FUNCTION__, "Uniform not found!");
}
#endif


template <GDL::U32 TNumProps>
std::vector<std::array<GLint, TNumProps>>
GDL::ProgramGL::FindProgramResourceData(GLenum eResourceType, std::array<GLenum, TNumProps> properties) const
{
    GLint numResources = 0;
    glGetProgramInterfaceiv(mHandle, eResourceType, GL_ACTIVE_RESOURCES, &numResources);
    std::vector<std::array<GLint, TNumProps>> results(numResources);
    for (GLint i = 0; i < numResources; ++i)
    {
        glGetProgramResourceiv(mHandle, eResourceType, i, TNumProps, properties.data(), TNumProps, nullptr,
                               results[i].data());
    }
    return results;
}

template <>
void GDL::ProgramGL::SetUniform<GL_FLOAT>(GLuint uniformLocation, GDL::F32 value) const
{
    assert(GetUniformByLocation(uniformLocation).GetType() == GL_FLOAT);
    glProgramUniform1f(mHandle, uniformLocation, value);
}


template <>
void GDL::ProgramGL::SetUniformArray<GL_FLOAT>(GLuint uniformLocation, const F32* const values, U32 size) const
{
    assert(GetUniformByLocation(uniformLocation).GetType() == GL_FLOAT);
    assert(GetUniformByLocation(uniformLocation).GetArraySize() >= size);
    glProgramUniform1fv(mHandle, uniformLocation, size, values);
}
