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
    , mPixelData(mWidth * mHeight * mNumChannels)
{
    std::memcpy(mPixelData.data(), data, mPixelData.size());
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



const Vector<U8>& TextureData2d::GetPixelData() const
{
    return mPixelData;
}



U32 TextureData2d::GetWidth() const
{
    return mWidth;
}



bool TextureData2d::IsDataValid() const
{
    return mWidth * mHeight * mNumChannels == mPixelData.size();
}

} // namespace GDL
