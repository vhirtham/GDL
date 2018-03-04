#include "shaderGLSL.h"

#include "base/Exception.h"

GDL::ShaderGLSL::~ShaderGLSL()
{
    glDeleteShader(mHandle);
}

GDL::ShaderGLSL::ShaderGLSL(GLenum shaderType, const char* shaderCode)
    : mShaderType(shaderType)
{
    mHandle = glCreateShader(mShaderType);
    glShaderSource(mHandle, 1, &shaderCode, nullptr);
    glCompileShader(mHandle);

    GLint status;
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(mHandle, infoLogLength, nullptr, infoLog);
        std::string errorMessage(infoLog);
        delete[] infoLog;

        throw Exception(__PRETTY_FUNCTION__,
                        "Failed to compile " + GetShaderTypeString(mShaderType) + "! \n\nLog Message:\n" +
                                errorMessage);
    }
}

std::string GDL::ShaderGLSL::GetShaderTypeString(GLenum shaderType)
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
