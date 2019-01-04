#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <utility>
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
    //! @param numLevels: Number of texture levels that should be created. 0 creates a level for each texture level in
    //! the provided texture data. If you provide a fixed number all levels that have a corresponding data set in the
    //! passed texture data will be initialized with this data. Levels without data will be left uninitialized.
    Texture(const TextureData2d& textureData, U32 numLevels = 0);

    //! @brief Binds the texture to the specified texture unit
    //! @param textureUnit: Texture unit
    void Bind(GLuint textureUnit) const;

    //! @brief Gets the textures handle
    //! @return The textures handle
    GLuint GetHandle() const;

    //! @brief Sets the border color for the corresponding wrapping method
    //! @param borderColor: Border color
    void SetBorderColor(std::array<F32, 4> borderColor);

    //! @brief Sets the filter that is used when magnifying the texture
    //! @param filter: Filter enum value
    void SetMagnifyingFilter(GLenum filter);

    //! @brief Sets the filter that is used when minifying the texture
    //! @param filter: Filter enum value
    void SetMinifyingFilter(GLenum filter);

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
    //! @param numLevels: Number of texture levels that should be created. 0 creates a level for each texture level in
    //! the provided texture data. If you provide a fixed number all levels that have a corresponding data set in the
    //! passed texture data will be initialized with this data. Levels without data will be left uninitialized.
    void Initialize(const TextureData2d& textureData, GLenum textureTarget, U32 numLevels);

    //! @brief Gets the texture format enums for a given number of channels
    //! @param numChannels: Number of channels of the texture
    //! @return Pair of texture format enums (format, internalFormat)
    //! @remark: Have a look at the OpenGL documentations of glTextureStorage2D and glTextureSubImage2D to understand
    //! the differences between both format enums
    static std::pair<GLenum, GLenum> GetTextureFormat(U32 numChannels);
};

} // namespace OpenGL
} // namespace GDL
