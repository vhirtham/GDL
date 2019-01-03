#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <GL/glew.h>


namespace GDL
{

class TextureData2d;

namespace OpenGL
{
//! @brief Wrapper class for OpenGL textures
class Texture
{

    GLuint mHandle;

public:
    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;
    ~Texture();

    //! @brief Ctor
    //! @param textureData: Texture data that should be used to create the texture object
    Texture(const TextureData2d& textureData);

    //! @brief Binds the texture to the specified texture unit
    //! @param textureUnit: Texture unit
    void Bind(GLuint textureUnit) const;

    //! @brief Gets the textures handle
    //! @return The textures handle
    GLuint GetHandle() const;

    //! @brief Sets the border color for the corresponding wrapping method
    //! @param borderColor: Border color
    void SetBorderColor(std::array<F32,4> borderColor);

    //! @brief Sets the wrapping in x-direction
    //! @param wrapping: Wrapping method enum
    void SetWrappingX(GLenum wrapping);

    //! @brief Sets the wrapping in y-direction
    //! @param wrapping: Wrapping method enum
    void SetWrappingY(GLenum wrapping);

    //! @brief Sets the wrapping in x- and y-direction
    //! @param wrapping: Wrapping method enum
    void SetWrappingXY(GLenum wrapping);

private:

    //! @brief Initializes the texture
    //! @param textureData: Texture data that should be used to initialize the texture object
    void Initialize(const TextureData2d& textureData, GLenum textureTarget);
};

} // namespace OpenGL
} // namespace GDL
