#include "gdl/rendering/textures/textureData2d.h"

#include "gdl/base/exception.h"

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



U32 TextureData2d::GetHeight() const
{
    return mHeight;
}



U32 TextureData2d::GetNumChannels() const
{
    return mNumChannels;
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



bool TextureData2d::IsDataValid() const
{
    return mWidth * mHeight * mNumChannels == mPixelData[0].size();
}

} // namespace GDL
