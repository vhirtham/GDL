#pragma once

#include <GL/glew.h>
#include <string>

namespace GDL
{

class ShaderGLSL
{
    GLuint mHandle = 0;
    GLenum mShaderType;

public:
    ShaderGLSL() = delete;
    ShaderGLSL(const ShaderGLSL&) = delete; // don't use default -> dtor calls glDeleteShader
    ShaderGLSL(ShaderGLSL&&) = delete; // don't use default -> dtor calls glDeleteShader
    ShaderGLSL& operator=(const ShaderGLSL&) = delete; // don't use default -> dtor calls glDeleteShader
    ShaderGLSL& operator=(ShaderGLSL&&) = delete; // don't use default -> dtor calls glDeleteShader
    ~ShaderGLSL();

    //! @brief Ctor which takes the Shader code and compiles it. Throws on compilation errors.
    //! @param shaderType: GLenum that corresponds to the shader type
    //! @param shaderCode: Shader code
    ShaderGLSL(GLenum shaderType, const char* shaderCode);

    //! @brief Gets the shaders handle
    //! @return Shader handle
    GLuint GetHandle() const
    {
        return mHandle;
    }


    //! @brief Returns the shader type name as a string
    //! @param shaderType: GLenum that corresponds to the shader type
    //! @return Shader type as a string
    static std::string GetShaderTypeString(GLenum shaderType);
};
}
