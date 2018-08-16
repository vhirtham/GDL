#include "programGL.h"

#include "gdl/base/exception.h"
#include "gdl/math/mat4.inl"
#include "gdl/rendering/openGL/core/shaderGL.h"

#include <cassert>
#include <memory>
#include <set>

GDL::OpenGL::Program::~Program()
{
    glDeleteProgram(mHandle);
}



GDL::OpenGL::Program::Program(std::initializer_list<std::reference_wrapper<const Shader>> shaderList)
    : mHandle(glCreateProgram())
{
    Initialize(shaderList);
}

GLuint GDL::OpenGL::Program::QueryUniformLocation(const std::string& uniformName) const
{
    GLint uniformLocation = glGetUniformLocation(mHandle, uniformName.data());
    assert(uniformLocation >= 0);
    return uniformLocation;
}

void GDL::OpenGL::Program::Use() const
{
    glUseProgram(mHandle);
}

void GDL::OpenGL::Program::CheckShaders(std::initializer_list<std::reference_wrapper<const Shader>> shaderList) const
{
    std::set<GLenum> attachedShaderTypes;
    for (auto& shader : shaderList)
    {
        if (attachedShaderTypes.find(shader.get().GetType()) != attachedShaderTypes.end())
            throw Exception(__PRETTY_FUNCTION__,
                            "Multiple shaders of the same type in one program are not "
                            "supported.\n You tried to attach multiple shaders of type: " +
                                    Shader::GetShaderTypeString(shader.get().GetType()));
        attachedShaderTypes.emplace(shader.get().GetType());
    }
    if (attachedShaderTypes.find(GL_VERTEX_SHADER) == attachedShaderTypes.end() ||
        attachedShaderTypes.find(GL_FRAGMENT_SHADER) == attachedShaderTypes.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "You have to provide a vertex shader and a fragment shader for each program!");
}



void GDL::OpenGL::Program::CheckLinkStatus() const
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

void GDL::OpenGL::Program::CheckForErrors() const
{
    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR)
        throw Exception(__PRETTY_FUNCTION__,
                        "Could not create program:\n" +
                                std::string(reinterpret_cast<const char*>(gluErrorString(errorCode))));
}


void GDL::OpenGL::Program::Initialize(std::initializer_list<std::reference_wrapper<const Shader>> shaderList)
{
    CheckShaders(shaderList);

    for (auto& shader : shaderList)
        glAttachShader(mHandle, shader.get().GetHandle());

    glLinkProgram(mHandle);

    for (auto& shader : shaderList)
        glDetachShader(mHandle, shader.get().GetHandle());

    CheckLinkStatus();
    CheckForErrors();
}



template <>
void GDL::OpenGL::Program::SetUniform(GLint uniformLocation, F32 value) const
{
    glProgramUniform1f(mHandle, uniformLocation, value);
}

template <>
void GDL::OpenGL::Program::SetUniform(GLint uniformLocation, Mat4f value) const
{
    glProgramUniformMatrix4fv(mHandle, uniformLocation, 1, GL_FALSE, value.Data().data());
}


template <>
void GDL::OpenGL::Program::SetUniformArray(GLint uniformLocation, const F32* const values, U32 size) const
{
    glProgramUniform1fv(mHandle, uniformLocation, size, values);
}
