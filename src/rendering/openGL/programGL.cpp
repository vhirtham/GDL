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
    FindUniforms();
}

void GDL::ProgramGL::FindUniforms()
{
    GLint numUniforms = 0;
    glGetProgramInterfaceiv(mHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

    constexpr U32 nProps = 3;
    GLenum quedProperties[nProps] = {GL_NAME_LENGTH, GL_LOCATION, GL_BLOCK_INDEX};
    GLint quedResults[nProps];

    for (I32 i = 0; i < numUniforms; i++)
    {
        glGetProgramResourceiv(mHandle, GL_UNIFORM, i, nProps, quedProperties, nProps, nullptr, quedResults);

        // if uniform is a block
        if (quedResults[2] != -1)
            continue;
        else
        // if uniform is not a block
        {
            GLint nameBufferSize = quedResults[0] + 1;
            std::unique_ptr<GLchar[]> uniformName = std::make_unique<GLchar[]>(nameBufferSize);
            glGetProgramResourceName(mHandle, GL_UNIFORM, i, nameBufferSize, nullptr, uniformName.get());
            mUniforms.emplace(std::string(uniformName.get()), quedResults[1]);
        }
    }
}
