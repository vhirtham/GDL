#include "gdl/rendering/openGL/core/shader.h"

#include "gdl/base/exception.h"
#include "gdl/base/container/vector.h"

namespace GDL::OpenGL
{


Shader::~Shader()
{
    glDeleteShader(mHandle);
}

Shader::Shader(GLenum shaderType, const char* shaderCode)
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

        Vector<GLchar> infoLog(static_cast<U32>(infoLogLength));
        glGetShaderInfoLog(mHandle, infoLogLength, nullptr, infoLog.data());

        THROW("Failed to compile " + std::string(GetShaderTypeString(mShaderType)) + "! \n\nLog Message:\n" +
              infoLog.data());
    }
}



GLuint Shader::GetHandle() const
{
    return mHandle;
}



GLenum Shader::GetType() const
{
    return mShaderType;
}



const char* Shader::GetShaderTypeString(GLenum shaderType)
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

} // namespace GDL::OpenGL
