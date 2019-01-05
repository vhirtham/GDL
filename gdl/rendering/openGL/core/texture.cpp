#include "gdl/rendering/openGL/core/texture.h"


#include "gdl/rendering/textures/textureData2d.h"

#include <cmath>


namespace GDL::OpenGL
{

Texture::Texture(const TextureData2d& textureData, U32 numLevels)
    : mHandle{0}
{
    Initialize(textureData, GL_TEXTURE_2D, numLevels);
}



void Texture::Bind(GLuint textureUnit) const
{
    glBindTextureUnit(textureUnit, mHandle);
}



GLuint Texture::GetHandle() const
{
    return mHandle;
}



U32 Texture::GetMaxNumTextureLevels2d(U32 width, U32 height)
{
    return static_cast<U32>(std::floor(std::log2(std::max(width, height))) + 1);
}



void Texture::SetBorderColor(std::array<F32, 4> borderColor)
{
    glTextureParameterfv(mHandle, GL_TEXTURE_BORDER_COLOR, borderColor.data());
}



void Texture::SetMagnifyingFilter(GLenum filter)
{
    glTextureParameteri(mHandle, GL_TEXTURE_MAG_FILTER, filter);
}



void Texture::SetMinifyingFilter(GLenum filter)
{
    glTextureParameteri(mHandle, GL_TEXTURE_MIN_FILTER, filter);
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



std::pair<GLenum, GLenum> Texture::GetTextureFormat(U32 numChannels)
{
    switch (numChannels)
    {
    case 1:
        return std::make_pair(GL_RED, GL_R8);
    case 2:
        return std::make_pair(GL_RG, GL_RG8);
    case 3:
        return std::make_pair(GL_RGB, GL_RGB8);
    case 4:
        return std::make_pair(GL_RGBA, GL_RGBA8);
    default:
        THROW("Unsupported number of channels");
    }
}



void Texture::Initialize(const TextureData2d& textureData, GLenum textureTarget, U32 numLevels)
{
    glCreateTextures(textureTarget, 1, &mHandle);


    auto [format, internalFormat] = GetTextureFormat(textureData.GetNumChannels());

    if (numLevels == 0)
        numLevels = textureData.GetNumTextureLevels();

    DEV_EXCEPTION(GetMaxNumTextureLevels2d(textureData.GetWidth(), textureData.GetHeight()) < numLevels,
                  "Number of texture levels too high");

    glTextureStorage2D(mHandle, numLevels, internalFormat, textureData.GetWidth(), textureData.GetHeight());

    for (U32 level = 0; level < std::min(numLevels, textureData.GetNumTextureLevels()); ++level)
        glTextureSubImage2D(mHandle, level, 0, 0, textureData.GetWidth(level), textureData.GetHeight(level), format,
                            GL_UNSIGNED_BYTE, textureData.GetPixelData(level).data());
}



} // namespace GDL::OpenGL
