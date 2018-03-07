#include "programGL.h"

#include "base/Exception.h"

#include <cassert>

GDL::ProgramGL::~ProgramGL()
{
    for (auto& shader : mShader)
    {
        glDetachShader(mHandle, shader.second.get().GetHandle());
    }
    glDeleteProgram(mHandle);
}



GDL::ProgramGL::ProgramGL(std::initializer_list<std::reference_wrapper<const ShaderGLSL>> shaderList)
{
    Initialize(shaderList);
}

void GDL::ProgramGL::CheckLinkStatus()
{
    GLint status;
    glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

    // error handling
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* infoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(mHandle, infoLogLength, nullptr, infoLog);

        std::string errorMessage(infoLog);
        delete[] infoLog;

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

    CheckLinkStatus();
}
