#pragma once


#include <GL/glew.h>

namespace GDL::OpenGL
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

    //! @brief Constructor which takes the Shader code and compiles it. Throws on compilation errors.
    //! @param shaderType: GLenum that corresponds to the shader type
    //! @param shaderCode: Shader code
    Shader(GLenum shaderType, const char* shaderCode);

    //! @brief Gets the shaders handle
    //! @return Shader handle
    GLuint GetHandle() const;

    //! @brief Gets the shaders type enum, e.g. vertex, fragment etc.
    //! @return Shader type
    GLenum GetType() const;

    //! @brief Returns the shader type name as a string
    //! @param shaderType: GLenum that corresponds to the shader type
    //! @return Shader type as a string
    static const char* GetShaderTypeString(GLenum shaderType);
};

} // namespace GDL::OpenGL
