#include "gdl/rendering/openGL/core/texture.h"


#include "gdl/rendering/textures/textureData.h"


namespace GDL::OpenGL
{

Texture::Texture(const TextureData& textureData)
    : mHandle{0}
{
    Initialize(textureData, GL_TEXTURE_2D);
}



void Texture::Bind(GLuint textureUnit) const
{
    glBindTextureUnit(textureUnit, mHandle);
}



GLuint Texture::GetHandle()
{
    return mHandle;
}



Texture::~Texture()
{
    glDeleteTextures(1, &mHandle);
}



void Texture::Initialize(const TextureData& textureData, GLenum textureTarget)
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
