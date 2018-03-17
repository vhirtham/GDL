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

GLuint GDL::ProgramGL::GetUniformHandle(std::string uniformName)
{
    auto iterator = mUniforms.find(uniformName);
#ifndef NDEBUG
    if (iterator == mUniforms.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find uniform \"" + uniformName +
                                "\". GLSL compiler optimization might be the reason.");
#endif
    return iterator->second.GetHandle();
}


template <>
void GDL::ProgramGL::SetUniformScalar(GLuint uniformHandle, F32 value)
{
    assert(GetUniformByHandle(uniformHandle).GetType() == GL_FLOAT);
    glProgramUniform1f(mHandle, uniformHandle, value);
}

template <>
void GDL::ProgramGL::SetUniformScalarArray(GLuint uniformHandle, std::vector<F32> values)
{
    assert(GetUniformByHandle(uniformHandle).GetType() == GL_FLOAT);
    assert(GetUniformByHandle(uniformHandle).GetSubsequentElementCount() >= values.size());
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

    glLinkProgram(mHandle);

    for (auto& shader : shaderList)
    {
        glDetachShader(mHandle, shader.get().GetHandle());
    }
    CheckLinkStatus();
    FindInputs();
    FindUniforms();
    FindUniformBlocks();
    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR)
        throw Exception(__PRETTY_FUNCTION__,
                        "Could not create program:\n" +
                                std::string(reinterpret_cast<const char*>(gluErrorString(errorCode))));
}



void GDL::ProgramGL::FindInputs()
{
    auto inputData = FindProgramResourceData<2>(GL_PROGRAM_INPUT, {{GL_LOCATION, GL_NAME_LENGTH}});
    for (U32 i = 0; i < inputData.size(); ++i)
        mProgramInputs.emplace(GetResourceName(GL_PROGRAM_INPUT, i, inputData[i][1]), inputData[i][0]);
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
    if (firstElement.GetSubsequentElementCount() > 1)
    {
        std::string arrayName = firstElementName;
        assert(arrayName.find("[0]", arrayName.length() - 3) == arrayName.length() - 3);
        arrayName.erase(arrayName.length() - 3, 3);

        for (GLint j = 1; j < firstElement.GetSubsequentElementCount(); ++j)
        {
            std::string arrayElementName = arrayName + "[" + std::to_string(j) + "]";
            GLint arrayElementHandle = glGetUniformLocation(mHandle, arrayElementName.c_str());

            if (arrayElementHandle < 0)
                throw Exception(__PRETTY_FUNCTION__,
                                "Could not find location of array element \"" + arrayElementName +
                                        "\"! This error might be caused by GLSL compiler optimizaion");

            mUniforms.emplace(arrayElementName, Uniform(arrayElementHandle, firstElement.GetType(),
                                                        firstElement.GetSubsequentElementCount() - j));
        }
    }
}

void GDL::ProgramGL::FindUniformBlocks()
{
    auto data = FindProgramResourceData<4>(
            GL_UNIFORM_BLOCK, {{GL_NAME_LENGTH, GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE, GL_NUM_ACTIVE_VARIABLES}});
    for (U32 i = 0; i < data.size(); ++i)
    {
        std::string uniformBlockName = GetResourceName(GL_UNIFORM_BLOCK, i, data[i][0]);
        GLint bufferBindingPoint = data[i][0];
        assert(glGetUniformBlockIndex(mHandle, uniformBlockName.c_str()) == i);

        std::vector<GLint> varIndices(data[i][3]);
        glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, varIndices.data());
        for (U32 j = 0; j < data[i][3]; ++j)
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
const GDL::Uniform& GDL::ProgramGL::GetUniformByHandle(GLuint handle)
{
    for (const auto& uniform : mUniforms)
        if (uniform.second.GetHandle() == handle)
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
