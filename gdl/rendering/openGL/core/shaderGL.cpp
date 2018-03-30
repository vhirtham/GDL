#include "shaderGL.h"

#include "gdl/base/Exception.h"

#include <memory>

GDL::OpenGL::Shader::~Shader()
{
    glDeleteShader(mHandle);
}

GDL::OpenGL::Shader::Shader(GLenum shaderType, const char* shaderCode)
    : mShaderType(shaderType)
    , mHandle(glCreateShader(mShaderType))
{
    glShaderSource(mHandle, 1, &shaderCode, nullptr);
    glCompileShader(mHandle);

    GLint status;
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::unique_ptr<GLchar[]> infoLog = std::make_unique<GLchar[]>(infoLogLength);
        glGetShaderInfoLog(mHandle, infoLogLength, nullptr, infoLog.get());

        throw Exception(__PRETTY_FUNCTION__,
                        "Failed to compile " + GetShaderTypeString(mShaderType) + "! \n\nLog Message:\n" +
                                infoLog.get());
    }
}

std::string GDL::OpenGL::Shader::GetShaderTypeString(GLenum shaderType)
{
    // clang-format off
    switch (shaderType)
    {
    case GL_VERTEX_SHADER:			return "Vertex Shader";
    case GL_TESS_CONTROL_SHADER:	return "Tesselation Control Shader";
    case GL_TESS_EVALUATION_SHADER: return "Tesselation Evaluation Shader";
    case GL_GEOMETRY_SHADER:		return "Geometry Shader";
    case GL_FRAGMENT_SHADER:		return "Fragment Shader";
    case GL_COMPUTE_SHADER:			return "Compute Shader";
    default:						return "UNKNOWN SHADER TYPE";
    }
    // clang-format on
}
