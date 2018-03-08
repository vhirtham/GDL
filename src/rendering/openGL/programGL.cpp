#include "programGL.h"

#include "base/Exception.h"

#include <cassert>

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
    GLenum quedProperties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};
    GLint quedResults[4];

    for (I32 i = 0; i < numUniforms; i++)
    {
        glGetProgramResourceiv(mHandle, GL_UNIFORM, i, 4, quedProperties, 4, nullptr, quedResults);

        // if uniform is a block
        if (quedResults[3] != -1)
            continue;
        else
        // if uniform is not a block
        {
            GLint nameBufferSize = quedResults[0] + 1;
            char* uniformName = new char[nameBufferSize];
            glGetProgramResourceName(mHandle, GL_UNIFORM, i, nameBufferSize, nullptr, uniformName);

            // insert into uniform map
            mUniforms.emplace(std::string(uniformName), quedResults[2]);

            delete[] uniformName;
        }
    }
}
