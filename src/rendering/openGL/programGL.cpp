#include "programGL.h"

#include "base/Exception.h"

#include <cassert>
#include <memory>

GDL::ProgramGL::~ProgramGL()
{
    glDeleteProgram(mHandle);
}



GDL::ProgramGL::ProgramGL(std::initializer_list<std::reference_wrapper<const ShaderGLSL>> shaderList)
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
    return iterator->second;
}

template <>
void GDL::ProgramGL::SetUniformScalar(GLuint uniformHandle, F32 value)
{
    glProgramUniform1f(mHandle, uniformHandle, value);
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

void GDL::ProgramGL::Initialize(std::initializer_list<std::reference_wrapper<const GDL::ShaderGLSL>> shaderList)
{
    mHandle = glCreateProgram();
    for (auto& shader : shaderList)
    {
        assert(mShader.find(shader.get().GetType()) == mShader.end() &&
               "Multiple shaders of the same type in one program are not supported");

        auto emplaceResult = mShader.emplace(shader.get().GetType(), shader);
        assert(emplaceResult.second == true && "Could not emplace shader into program data");

        glAttachShader(mHandle, emplaceResult.first->second.get().GetHandle());
    }

    glLinkProgram(mHandle);

    for (auto& shader : mShader)
    {
        glDetachShader(mHandle, shader.second.get().GetHandle());
    }
    CheckLinkStatus();
    FindInputs();
    FindUniforms();
}



void GDL::ProgramGL::FindInputs()
{
    auto inputData = FindProgramResourceData<2>(GL_PROGRAM_INPUT, {{GL_LOCATION, GL_NAME_LENGTH}});
    for (U32 i = 0; i < inputData.size(); ++i)
        mProgramInputs.emplace(GetResourceName(GL_PROGRAM_INPUT, inputData[i][0], inputData[i][1]), inputData[i][0]);
}



void GDL::ProgramGL::FindUniforms()
{
    auto uniformData = FindProgramResourceData<3>(GL_UNIFORM, {{GL_LOCATION, GL_NAME_LENGTH, GL_BLOCK_INDEX}});
    for (U32 i = 0; i < uniformData.size(); ++i)
        if (uniformData[i][2] != -1) // if uniform is a block
            continue;
        else // if uniform is NOT a block
            mUniforms.emplace(GetResourceName(GL_UNIFORM, uniformData[i][0], uniformData[i][1]), uniformData[i][0]);
}



std::string GDL::ProgramGL::GetResourceName(GLenum eResourceType, GLint location, GLint bufferSize)
{
    std::unique_ptr<GLchar[]> resourceName = std::make_unique<GLchar[]>(bufferSize);
    glGetProgramResourceName(mHandle, eResourceType, location, bufferSize, nullptr, resourceName.get());
    return std::string(resourceName.get());
}



template <GDL::U32 TNumProps>
std::vector<std::array<GLint, TNumProps>>
GDL::ProgramGL::FindProgramResourceData(GLenum eResourceType, std::array<GLenum, TNumProps> properties)
{
    GLint numResources = 0;
    glGetProgramInterfaceiv(mHandle, eResourceType, GL_ACTIVE_RESOURCES, &numResources);
    std::vector<std::array<GLint, TNumProps>> results(numResources);
    for (I32 i = 0; i < numResources; ++i)
    {
        glGetProgramResourceiv(mHandle, eResourceType, i, TNumProps, properties.data(), TNumProps, nullptr,
                               results[i].data());
    }
    return results;
}
