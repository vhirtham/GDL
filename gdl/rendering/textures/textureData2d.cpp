#include "gdl/rendering/textures/textureData2d.h"

#include "gdl/base/exception.h"
#include "gdl/base/functions/isPowerOf2.h"

#include <cmath>
#include <cstring>



namespace GDL
{

TextureData2d::TextureData2d()
    : mNumChannels{0}
    , mWidth{0}
    , mHeight{0}

{
}



TextureData2d::TextureData2d(U32 width, U32 height, U32 numChannels, const Vector<U8>& data)
    : mNumChannels{numChannels}
    , mWidth{width}
    , mHeight{height}
    , mPixelData{data}
{
    DEV_EXCEPTION(!IsDataValid(), "Data size and texture attributes are not compatible.");
}



TextureData2d::TextureData2d(U32 width, U32 height, U32 numChannels, const U8* data)
    : mNumChannels{numChannels}
    , mWidth{width}
    , mHeight{height}
{
    mPixelData.emplace_back(mWidth[0] * mHeight[0] * mNumChannels);
    std::memcpy(mPixelData[0].data(), data, mPixelData[0].size());
    DEV_EXCEPTION(!IsDataValid(), "Data size and texture attributes are not compatible.");
}



void TextureData2d::CreateMipMaps(Interpolation interpolation)
{
    DEV_EXCEPTION(mPixelData.size() != 1, "Texture has more than 1 level. Mipmaps already created?");


    U32 currentLevel = 0;
    while (mWidth[currentLevel] != 1 && mHeight[currentLevel] != 1)
    {
        mWidth.emplace_back(std::max<U32>(1, mWidth[currentLevel] / 2));
        mHeight.emplace_back(std::max<U32>(1, mHeight[currentLevel] / 2));
        ++currentLevel;

        mPixelData.emplace_back(mWidth[currentLevel] * mHeight[currentLevel] * mNumChannels);
        switch (interpolation)
        {
        case Interpolation::NONE:
            std::memset(mPixelData[currentLevel].data(), 0, mPixelData[currentLevel].size());
            break;
        case Interpolation::LINEAR:
            CalculateLinearMipMap(currentLevel);
            break;
        }
    }

    DEV_EXCEPTION(!IsDataValid(), "Calculated mipmap data is corrupted.");
}



U32 TextureData2d::GetHeight(U32 level) const
{
    DEV_EXCEPTION(level >= mPixelData.size(), "Texture does not have the requested texture level");
    return mHeight[level];
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



U32 TextureData2d::GetWidth(U32 level) const
{
    DEV_EXCEPTION(level >= mPixelData.size(), "Texture does not have the requested texture level");
    return mWidth[level];
}



bool TextureData2d::IsDataValid() const
{
    for (U32 i = 0; i < mPixelData.size(); ++i)
        if (mWidth[i] * mHeight[i] * mNumChannels != mPixelData[i].size())
            return false;
    return true;
}



void TextureData2d::CalculateLinearMipMap(U32 level)
{
    DEV_EXCEPTION(level < 1, "Invalid mipmap level.");
    DEV_EXCEPTION(IsPowerOf2(!mWidth[0]), "Texture width is not a power of 2");
    DEV_EXCEPTION(IsPowerOf2(!mHeight[0]), "Texture height is not a power of 2");

    const U32 prevLevel = level - 1;

    for (U32 i = 0; i < mWidth[level]; ++i)
        for (U32 j = 0; j < mHeight[level]; ++j)
            for (U32 k = 0; k < mNumChannels; ++k)
            {
                const U32 pixelIndexCurrent = (i * mWidth[level] + j) * mNumChannels + k;
                const U32 pixelIndexPrev0 = (i * mWidth[prevLevel] + j) * 2 * mNumChannels + k;
                const U32 pixelIndexPrev1 = pixelIndexPrev0 + mNumChannels;
                const U32 pixelIndexPrev2 = pixelIndexPrev0 + mWidth[prevLevel] * mNumChannels;
                const U32 pixelIndexPrev3 = pixelIndexPrev2 + mNumChannels;

                mPixelData[level][pixelIndexCurrent] =
                        (mPixelData[prevLevel][pixelIndexPrev0] + mPixelData[prevLevel][pixelIndexPrev1] +
                         mPixelData[prevLevel][pixelIndexPrev2] + mPixelData[prevLevel][pixelIndexPrev3]) /
                        4;
            }
}


} // namespace GDL
