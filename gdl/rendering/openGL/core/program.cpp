#include "gdl/rendering/openGL/core/program.h"

#include "gdl/base/exception.h"
#include "gdl/base/string.h"
#include "gdl/base/container/vector.h"
#include "gdl/math/serial/mat4Serial.h"
#include "gdl/math/serial/vec3Serial.h"
#include "gdl/math/serial/vec4Serial.h"
#include "gdl/math/sse/mat4fAVX.h"
#include "gdl/math/sse/mat4fSSE.h"
#include "gdl/math/sse/vec3fSSE.h"
#include "gdl/math/sse/vec4fSSE.h"
#include "gdl/rendering/openGL/core/shader.h"



namespace GDL::OpenGL
{

Program::~Program()
{
    glDeleteProgram(mHandle);
}



Program::Program(std::initializer_list<std::reference_wrapper<const Shader>> shaderList)
    : mHandle(glCreateProgram())
{
    Initialize(shaderList);
}



GLuint Program::GetHandle() const
{
    return mHandle;
}



bool Program::QueryHasUniform(const char* uniformName) const
{
    return 0 < glGetUniformLocation(mHandle, uniformName);
}



GLuint Program::QueryUniformBlockBinding(GLuint uniformBlockIndex) const
{
    GLint blockBinding = -1;
    glGetActiveUniformBlockiv(mHandle, uniformBlockIndex, GL_UNIFORM_BLOCK_BINDING, &blockBinding);
    DEV_EXCEPTION(blockBinding < 0, "Could not query uniform block binding.");
    return static_cast<GLuint>(blockBinding);
}



GLuint Program::QueryUniformBlockIndex(const char* uniformBlockName) const
{
    GLuint blockIndex = glGetUniformBlockIndex(mHandle, uniformBlockName);
    DEV_EXCEPTION(blockIndex == GL_INVALID_INDEX,
                  MakeString("Could not find uniform with name \"", uniformBlockName, "\"").c_str());
    return blockIndex;
}



GLint Program::QueryUniformBlockSize(GLuint uniformBlockIndex) const
{
    GLint blockSize = -1;
    glGetActiveUniformBlockiv(mHandle, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    DEV_EXCEPTION(blockSize < 0, "Could not query uniform block size.");
    return blockSize;
}



GLint Program::QueryUniformLocation(const char* uniformName) const
{
    GLint uniformLocation = glGetUniformLocation(mHandle, uniformName);
    DEV_EXCEPTION(uniformLocation < 0, MakeString("Could not find uniform with name \"", uniformName, "\"").c_str());
    return uniformLocation;
}



void Program::Use() const
{
    glUseProgram(mHandle);
}



void Program::CheckLinkStatus() const
{
    GLint status;
    glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

        Vector<GLchar> infoLog(static_cast<U32>(infoLogLength));
        glGetProgramInfoLog(mHandle, infoLogLength, nullptr, infoLog.data());

        THROW(MakeString("Failed to compile shader program \n\nLog Message:\n", infoLog.data()).c_str());
    }
}



void Program::CheckForErrors() const
{
    GLenum errorCode = glGetError();
    EXCEPTION(errorCode != GL_NO_ERROR,
              MakeString("Could not create program:\n", reinterpret_cast<const char*>(gluErrorString(errorCode)))
                      .c_str());
}



void Program::Initialize(std::initializer_list<std::reference_wrapper<const Shader>> shaderList)
{

    for (auto& shader : shaderList)
        glAttachShader(mHandle, shader.get().GetHandle());

    glLinkProgram(mHandle);

    for (auto& shader : shaderList)
        glDetachShader(mHandle, shader.get().GetHandle());

    CheckLinkStatus();
    CheckForErrors();
}



void Program::SetUniform(GLint uniformLocation, F32 value)
{
    glProgramUniform1f(mHandle, uniformLocation, value);
}



void Program::SetUniform(GLint uniformLocation, I32 value)
{
    glProgramUniform1i(mHandle, uniformLocation, value);
}



void Program::SetUniform(GLint uniformLocation, U32 value)
{
    glProgramUniform1ui(mHandle, uniformLocation, value);
}



void Program::SetUniform(GLint uniformLocation, const std::array<F32, 2>& values)
{
    glProgramUniform2fv(mHandle, uniformLocation, 1, values.data());
}



void Program::SetUniform(GLint uniformLocation, const Vec3Serial<F32, true>& values)
{
    glProgramUniform3fv(mHandle, uniformLocation, 1, values.Data().data());
}



void Program::SetUniform(GLint uniformLocation, const Vec3Serial<F32, false>& values)
{
    glProgramUniform3fv(mHandle, uniformLocation, 1, values.Data().data());
}



void Program::SetUniform(GLint uniformLocation, const Vec3fSSE<true>& values)
{
    glProgramUniform3fv(mHandle, uniformLocation, 1, values.Data().data());
}



void Program::SetUniform(GLint uniformLocation, const Vec3fSSE<false>& values)
{
    glProgramUniform3fv(mHandle, uniformLocation, 1, values.Data().data());
}



void Program::SetUniform(GLint uniformLocation, const Vec4Serial<F32, true>& values)
{
    glProgramUniform4fv(mHandle, uniformLocation, 1, values.Data().data());
}



void Program::SetUniform(GLint uniformLocation, const Vec4Serial<F32, false>& values)
{
    glProgramUniform4fv(mHandle, uniformLocation, 1, values.Data().data());
}



void Program::SetUniform(GLint uniformLocation, const Vec4fSSE<true>& values)
{
    glProgramUniform4fv(mHandle, uniformLocation, 1, values.Data().data());
}



void Program::SetUniform(GLint uniformLocation, const Vec4fSSE<false>& values)
{
    glProgramUniform4fv(mHandle, uniformLocation, 1, values.Data().data());
}



void Program::SetUniform(GLint uniformLocation, const Mat4fSSE& values)
{
    glProgramUniformMatrix4fv(mHandle, uniformLocation, 1, GL_FALSE, values.Data().data());
}



#ifdef __AVX2__

void Program::SetUniform(GLint uniformLocation, const Mat4fAVX& values)
{
    glProgramUniformMatrix4fv(mHandle, uniformLocation, 1, GL_FALSE, values.Data().data());
}

#endif //__AVX2__



void Program::SetUniform(GLint uniformLocation, const Mat4Serial<F32>& values)
{
    glProgramUniformMatrix4fv(mHandle, uniformLocation, 1, GL_FALSE, values.Data().data());
}



void Program::SetUniformArray(GLint uniformLocation, const F32* const values, U32 size)
{
    glProgramUniform1fv(mHandle, uniformLocation, size, values);
}



void Program::SetUniformBlockBinding(GLuint uniformBlockIndex, GLuint bindingPoint)
{
    glUniformBlockBinding(mHandle, uniformBlockIndex, bindingPoint);
}



void Program::SetUniformSamplerTextureUnit(const char* uniformName, U32 textureUnit)
{
    GLint uniformLocation = QueryUniformLocation(uniformName);
    SetUniform(uniformLocation, static_cast<I32>(textureUnit));
}



} // namespace GDL::OpenGL
