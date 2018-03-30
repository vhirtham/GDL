#include "programGL.h"

#include "gdl/base/Exception.h"
#include "gdl/rendering/openGL/core/shaderGL.h"

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

void GDL::ProgramGL::CheckShaders(std::initializer_list<std::reference_wrapper<const GDL::ShaderGL>> shaderList) const
{
    std::set<GLenum> attachedShaderTypes;
    for (auto& shader : shaderList)
    {
        if (attachedShaderTypes.find(shader.get().GetType()) != attachedShaderTypes.end())
            throw Exception(__PRETTY_FUNCTION__,
                            "Multiple shaders of the same type in one program are not "
                            "supported.\n You tried to attach multiple shaders of type: " +
                                    ShaderGL::GetShaderTypeString(shader.get().GetType()));
        attachedShaderTypes.emplace(shader.get().GetType());
    }
    if (attachedShaderTypes.find(GL_VERTEX_SHADER) == attachedShaderTypes.end() ||
        attachedShaderTypes.find(GL_FRAGMENT_SHADER) == attachedShaderTypes.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "You have to provide a vertex shader and a fragment shader for each program!");
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

void GDL::ProgramGL::CheckForErrors() const
{
    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR)
        throw Exception(__PRETTY_FUNCTION__,
                        "Could not create program:\n" +
                                std::string(reinterpret_cast<const char*>(gluErrorString(errorCode))));
}


void GDL::ProgramGL::Initialize(std::initializer_list<std::reference_wrapper<const GDL::ShaderGL>> shaderList)
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
void GDL::ProgramGL::SetUniform<GL_FLOAT>(GLuint uniformLocation, GDL::F32 value) const
{
    glProgramUniform1f(mHandle, uniformLocation, value);
}


template <>
void GDL::ProgramGL::SetUniformArray<GL_FLOAT>(GLuint uniformLocation, const F32* const values, U32 size) const
{
    glProgramUniform1fv(mHandle, uniformLocation, size, values);
}
