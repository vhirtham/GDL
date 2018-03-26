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

GDL::ProgramInput GDL::ProgramGL::GetInput(std::string inputName) const
{
    auto iterator = mInputs.find(inputName);
    if (iterator == mInputs.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find input \"" + inputName + "\". GLSL compiler optimization might be the reason.");
    return iterator->second;
}

GDL::Uniform GDL::ProgramGL::GetUniform(std::string uniformName) const
{
    auto iterator = mUniforms.find(uniformName);
    if (iterator == mUniforms.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find uniform \"" + uniformName +
                                "\". GLSL compiler optimization might be the reason.");
    return iterator->second;
}


template <>
void GDL::ProgramGL::SetUniformScalar(GLuint uniformHandle, F32 value)
{
    assert(GetUniformByLocation(uniformHandle).GetType() == GL_FLOAT);
    glProgramUniform1f(mHandle, uniformHandle, value);
}

template <>
void GDL::ProgramGL::SetUniformScalarArray(GLuint uniformHandle, std::vector<F32> values)
{
    assert(GetUniformByLocation(uniformHandle).GetType() == GL_FLOAT);
    assert(GetUniformByLocation(uniformHandle).GetSubsequentArraySize() >= values.size());
    glProgramUniform1fv(mHandle, uniformHandle, values.size(), values.data());
}

void GDL::ProgramGL::CheckLinkStatus()
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
                                          Uniform(data[i][0], data[i][2], data[i][3], data[i][4]))
                                 .first;

        FindUniformArrayMembers(uniformIt->first, uniformIt->second);
    }
}

void GDL::ProgramGL::FindUniformArrayMembers(const std::string& firstElementName, const Uniform& firstElement)
{
    if (firstElement.GetSubsequentArraySize() > 1)
    {
        std::string arrayName = firstElementName;
        assert(arrayName.find("[0]", arrayName.length() - 3) == arrayName.length() - 3);
        arrayName.erase(arrayName.length() - 3, 3);

        for (GLuint j = 1; j < firstElement.GetSubsequentArraySize(); ++j)
        {
            std::string arrayElementName = arrayName + "[" + std::to_string(j) + "]";
            GLint arrayElementHandle = glGetUniformLocation(mHandle, arrayElementName.c_str());

            if (arrayElementHandle < 0)
                throw Exception(__PRETTY_FUNCTION__,
                                "Could not find location of array element \"" + arrayElementName +
                                        "\"! This error might be caused by GLSL compiler optimizaion");

            mUniforms.emplace(arrayElementName,
                              Uniform(arrayElementHandle, firstElement.GetType(),
                                      firstElement.GetSubsequentArraySize() - j, firstElement.GetBlockIndex()));
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
                mUniformBlocks.emplace(uniformBlockName, UniformBlock(i, data[i][1], data[i][2], data[i][3]))
                        .first->second;

        // Put the following stuff in own function!
        std::vector<GLint> varIndices(data[i][2]);
        glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, varIndices.data());
        for (GLint j = 0; j < data[i][2]; ++j)
        {
            GLuint uniformIndex = static_cast<GLuint>(varIndices[j]);
            GLint uniformNameLength;
            glGetActiveUniformsiv(mHandle, 1, &uniformIndex, GL_UNIFORM_NAME_LENGTH, &uniformNameLength);
            std::string uniformName = GetResourceName(GL_UNIFORM, uniformIndex, uniformNameLength);
            GLint uniformOffset;
            glGetActiveUniformsiv(mHandle, 1, &uniformIndex, GL_UNIFORM_OFFSET, &uniformOffset);
            GLint uniformSize;
            glGetActiveUniformsiv(mHandle, 1, &uniformIndex, GL_UNIFORM_SIZE, &uniformSize);
            int a = 0;
        }
    }
}



std::string GDL::ProgramGL::GetResourceName(GLenum eResourceType, GLuint index, GLint bufferSize)
{
    std::unique_ptr<GLchar[]> resourceName = std::make_unique<GLchar[]>(bufferSize);
    glGetProgramResourceName(mHandle, eResourceType, index, bufferSize, nullptr, resourceName.get());
    std::string nameString(resourceName.get());
    return nameString;
}

#ifndef NDEBUG
const GDL::Uniform& GDL::ProgramGL::GetUniformByLocation(GLint location)
{
    for (const auto& uniform : mUniforms)
        if (uniform.second.GetLocation() == location)
            return uniform.second;

    throw Exception(__PRETTY_FUNCTION__, "Uniform not found!");
}
#endif


template <GDL::U32 TNumProps>
std::vector<std::array<GLint, TNumProps>>
GDL::ProgramGL::FindProgramResourceData(GLenum eResourceType, std::array<GLenum, TNumProps> properties)
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
void GDL::ProgramGL::SetUniform<GL_FLOAT>(GLuint uniformHandle, GDL::F32 value)
{
    assert(GetUniformByLocation(uniformHandle).GetType() == GL_FLOAT);
    glProgramUniform1f(mHandle, uniformHandle, value);
}
