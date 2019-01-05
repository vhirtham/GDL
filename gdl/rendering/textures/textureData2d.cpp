#include "gdl/rendering/textures/textureData2d.h"

#include "gdl/base/exception.h"
#include "gdl/base/functions/isPowerOf2.h"
#include "gdl/base/functions/pow.h"

#include <cmath>
#include <cstring>



namespace GDL
{

TextureData2d::TextureData2d()
    : mWidth{0}
    , mHeight{0}
    , mNumChannels{0}

{
}



TextureData2d::TextureData2d(U32 width, U32 height, U32 numChannels, const Vector<U8>& data)
    : mWidth{width}
    , mHeight{height}
    , mNumChannels{numChannels}
    , mPixelData{data}
{
    DEV_EXCEPTION(!IsDataValid(), "Data size and texture attributes are not compatible.");
}



TextureData2d::TextureData2d(U32 width, U32 height, U32 numChannels, const U8* data)
    : mWidth{width}
    , mHeight{height}
    , mNumChannels{numChannels}
{
    mPixelData.emplace_back(mWidth * mHeight * mNumChannels);
    std::memcpy(mPixelData[0].data(), data, mPixelData[0].size());
    DEV_EXCEPTION(!IsDataValid(), "Data size and texture attributes are not compatible.");
}



void TextureData2d::CreateMipMaps(Interpolation interpolation)
{
    DEV_EXCEPTION(mPixelData.size() != 1, "Texture has more than 1 level. Mipmaps already created?");


    U32 currentLevel = 0;
    U32 currentWidth = mWidth;
    U32 currentHeight = mHeight;

    while (currentWidth != 1 || currentHeight != 1)
    {
        currentWidth = std::max<U32>(1, currentWidth / 2);
        currentHeight = std::max<U32>(1, currentHeight / 2);
        ++currentLevel;

        mPixelData.emplace_back(currentWidth * currentHeight * mNumChannels);
        switch (interpolation)
        {
        case Interpolation::NONE:
            std::memset(mPixelData[currentLevel].data(), 0, mPixelData[currentLevel].size());
            break;
        case Interpolation::LINEAR:
            CalculateLinearMipMap(currentLevel, currentWidth, currentHeight);
            break;
        }
    }

    DEV_EXCEPTION(!IsDataValid(), "Calculated mipmap data is corrupted.");
}



U32 TextureData2d::GetHeight() const
{
    return mHeight;
}



U32 TextureData2d::GetHeight(U32 level) const
{
    DEV_EXCEPTION(level >= mPixelData.size(), "Texture does not have the requested texture level");
    return std::max<U32>(1, mHeight / Pow<U32>(2, static_cast<I32>(level)));
}



U32 TextureData2d::GetNumChannels() const
{
    return mNumChannels;
}



U32 TextureData2d::GetNumTextureLevels() const
{
    return static_cast<U32>(mPixelData.size());
}



const Vector<U8>& TextureData2d::GetPixelData(U32 level) const
{
    DEV_EXCEPTION(level >= mPixelData.size(), "Texture does not have the requested texture level");
    return mPixelData[level];
}



U32 TextureData2d::GetWidth() const
{
    return mWidth;
}



U32 TextureData2d::GetWidth(U32 level) const
{
    DEV_EXCEPTION(level >= mPixelData.size(), "Texture does not have the requested texture level");
    return std::max<U32>(1, mWidth / Pow<U32>(2, static_cast<I32>(level)));
}



bool TextureData2d::IsDataValid() const
{
    if (mWidth == 0 || mHeight == 0)
        return false;

    for (U32 i = 0; i < mPixelData.size(); ++i)
        if (GetWidth(i) * GetHeight(i) * mNumChannels != mPixelData[i].size())
            return false;
    return true;
}



void TextureData2d::SetMipMapData(U32 level, Vector<U8> pixelData)
{
    DEV_EXCEPTION(level < 1 || level >= mPixelData.size(), "Invalid mipmap level");
    DEV_EXCEPTION(pixelData.size() != mPixelData[level].size(), "Provided pixel data has wrong size");
    std::memcpy(mPixelData[level].data(), pixelData.data(), mPixelData[level].size());
}



void TextureData2d::CalculateLinearMipMap(U32 level, U32 width, U32 height)
{
    DEV_EXCEPTION(level < 1, "Invalid mipmap level.");
    DEV_EXCEPTION(IsPowerOf2(!mWidth), "Texture width is not a power of 2");
    DEV_EXCEPTION(IsPowerOf2(!mHeight), "Texture height is not a power of 2");

    const U32 prevLevel = level - 1;
    const U32 prevWidth = GetWidth(prevLevel);

    for (U32 i = 0; i < width; ++i)
        for (U32 j = 0; j < height; ++j)
            for (U32 k = 0; k < mNumChannels; ++k)
            {
                const U32 pixelIndexCurrent = (i * width + j) * mNumChannels + k;
                const U32 pixelIndexPrev0 = (i * prevWidth + j) * 2 * mNumChannels + k;
                const U32 pixelIndexPrev1 = pixelIndexPrev0 + mNumChannels;
                const U32 pixelIndexPrev2 = pixelIndexPrev0 + prevWidth * mNumChannels;
                const U32 pixelIndexPrev3 = pixelIndexPrev2 + mNumChannels;

                mPixelData[level][pixelIndexCurrent] =
                        (mPixelData[prevLevel][pixelIndexPrev0] + mPixelData[prevLevel][pixelIndexPrev1] +
                         mPixelData[prevLevel][pixelIndexPrev2] + mPixelData[prevLevel][pixelIndexPrev3]) /
                        4;
            }
}


} // namespace GDL
