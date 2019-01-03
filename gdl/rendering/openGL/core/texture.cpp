#include "gdl/rendering/openGL/core/texture.h"


#include "gdl/rendering/textures/textureData2d.h"


namespace GDL::OpenGL
{

Texture::Texture(const TextureData2d& textureData)
    : mHandle{0}
{
    Initialize(textureData, GL_TEXTURE_2D);
}



void Texture::Bind(GLuint textureUnit) const
{
    glBindTextureUnit(textureUnit, mHandle);
}



GLuint Texture::GetHandle() const
{
    return mHandle;
}



void Texture::SetBorderColor(std::array<F32, 4> borderColor)
{
    glTextureParameterfv(mHandle, GL_TEXTURE_BORDER_COLOR, borderColor.data());
}



void Texture::SetWrappingX(GLenum wrapping)
{
    glTextureParameteri(mHandle, GL_TEXTURE_WRAP_S, wrapping);
}



void Texture::SetWrappingY(GLenum wrapping)
{
    glTextureParameteri(mHandle, GL_TEXTURE_WRAP_T, wrapping);
}

void Texture::SetWrappingXY(GLenum wrapping)
{
    SetWrappingX(wrapping);
    SetWrappingY(wrapping);
}



Texture::~Texture()
{
    glDeleteTextures(1, &mHandle);
}



void Texture::Initialize(const TextureData2d& textureData, GLenum textureTarget)
{
    glCreateTextures(textureTarget, 1, &mHandle);

    GLenum format = GL_NONE;
    GLenum internalFormat = GL_NONE;

    switch (textureData.GetNumChannels())
    {
    case 1:
        format = GL_RED;
        internalFormat = GL_R8;
        break;
    case 2:
        format = GL_RG;
        internalFormat = GL_RG8;
        break;
    case 3:
        format = GL_RGB;
        internalFormat = GL_RGB8;
        break;
    case 4:
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
        break;
    default:
        THROW("The texture data contains an unsupported number of channels");
    }

    glTextureStorage2D(mHandle, 1, internalFormat, textureData.GetWidth(), textureData.GetHeight());
    glTextureSubImage2D(mHandle, 0, 0, 0, textureData.GetWidth(), textureData.GetHeight(), format, GL_UNSIGNED_BYTE,
                        textureData.GetPixelData().data());
}

} // namespace GDL::OpenGL
