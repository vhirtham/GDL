#pragma once

#include <GL/glew.h>
#include <string>

namespace GDL
{
namespace OpenGL
{
class Shader
{
    const GLenum mShaderType;
    const GLuint mHandle = 0;

public:
    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;
    ~Shader();

    //! @brief Ctor which takes the Shader code and compiles it. Throws on compilation errors.
    //! @param shaderType: GLenum that corresponds to the shader type
    //! @param shaderCode: Shader code
    Shader(GLenum shaderType, const char* shaderCode);

    //! @brief Gets the shaders handle
    //! @return Shader handle
    GLuint GetHandle() const
    {
        return mHandle;
    }

    //! @brief Gets the shaders type enum, e.g. vertex, fragment etc.
    //! @return Shader type
    GLenum GetType() const
    {
        return mShaderType;
    }


    //! @brief Returns the shader type name as a string
    //! @param shaderType: GLenum that corresponds to the shader type
    //! @return Shader type as a string
    static std::string GetShaderTypeString(GLenum shaderType);
};
}
}
