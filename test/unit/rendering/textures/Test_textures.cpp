#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/rendering/textures/textureData2d.h"
#include "gdl/rendering/textures/textureFile.h"

#include "test/tools/ExceptionChecks.h"


using namespace GDL;



BOOST_AUTO_TEST_CASE(Texture_Data)
{
    constexpr U32 width = 1;
    constexpr U32 height = 4;
    constexpr U32 numChannels = 3;

    // clang-format off
    Vector<U8> data = { 255,   0,   0,
                          0, 255,   0,
                          0,   0, 255,
                        255, 255, 255};
    // clang-format on

    TextureData2d textureData(width, height, numChannels, data);
    BOOST_CHECK(textureData.GetWidth() == width);
    BOOST_CHECK(textureData.GetHeight() == height);
    BOOST_CHECK(textureData.GetNumChannels() == numChannels);
    for (U32 i = 0; i < textureData.GetPixelData().size(); ++i)
        BOOST_CHECK(textureData.GetPixelData()[i] == data[i]);


    // Test ctor with U8 ptr
    TextureData2d textureData2(width, height, numChannels, data.data());
    BOOST_CHECK(textureData2.GetWidth() == width);
    BOOST_CHECK(textureData2.GetHeight() == height);
    BOOST_CHECK(textureData2.GetNumChannels() == numChannels);
    for (U32 i = 0; i < textureData2.GetPixelData().size(); ++i)
        BOOST_CHECK(textureData2.GetPixelData()[i] == data[i]);


    GDL_CHECK_THROW_DEV_DISABLE(TextureData2d(0, 4, 3, data), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(TextureData2d(4, 0, 3, data), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(TextureData2d(2, 6, 0, data), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(TextureData2d(2, 3, 3, data), Exception);
}



BOOST_AUTO_TEST_CASE(Texture_Data_Mipmaps)
{
    constexpr U32 width = 2;
    constexpr U32 height = 2;
    constexpr U32 numChannels = 3;

    // clang-format off
    Vector<U8> data = { 254,   0,   0,
                          0, 254,   0,
                          0,   0, 254,
                        254, 254, 254};
    // clang-format on

    TextureData2d textureData(width, height, numChannels, data);
    BOOST_CHECK(textureData.GetNumTextureLevels() == 1);
    GDL_CHECK_THROW_DEV_DISABLE(textureData.GetWidth(1), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(textureData.GetHeight(1), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(textureData.GetPixelData(1), Exception);

    textureData.CreateMipMaps(Interpolation::LINEAR);
    GDL_CHECK_THROW_DEV_DISABLE(textureData.CreateMipMaps(), Exception);

    BOOST_CHECK(textureData.GetNumTextureLevels() == 2);
    BOOST_CHECK(textureData.GetWidth(1) == 1);
    BOOST_CHECK(textureData.GetHeight(1) == 1);

    Vector<U8> pixelData = textureData.GetPixelData(1);
    BOOST_CHECK(pixelData.size() == numChannels);
    for (U32 i = 0; i < pixelData.size(); ++i)
        BOOST_CHECK(pixelData[i] == 254 / 2);

    TextureData2d textureData2(width, height, numChannels, data);
    textureData2.CreateMipMaps(Interpolation::NONE);
    Vector<U8> pixelData2 = textureData2.GetPixelData(1);
    BOOST_CHECK(pixelData2.size() == numChannels);
    for (U32 i = 0; i < pixelData2.size(); ++i)
        BOOST_CHECK(pixelData2[i] == 0);

    textureData.SetMipMapData(1, pixelData2);
    Vector<U8> pixelData3 = textureData.GetPixelData(1);
    BOOST_CHECK(pixelData3.size() == numChannels);
    for (U32 i = 0; i < pixelData3.size(); ++i)
        BOOST_CHECK(pixelData3[i] == 0);

    TextureData2d textureData3(width, height, numChannels, data);
    GDL_CHECK_THROW_DEV_DISABLE(textureData3.SetMipMapData(0, textureData.GetPixelData()), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(textureData3.SetMipMapData(1, textureData.GetPixelData(1)), Exception);
    textureData3.CreateMipMaps(Interpolation::NONE);
    GDL_CHECK_THROW_DEV_DISABLE(textureData3.SetMipMapData(1, textureData.GetPixelData(0)), Exception);
}



BOOST_AUTO_TEST_CASE(Texture_File)
{
    // clang-format off
    Vector<U8> data = { 255,   0,   0,
                          0, 255,   0,
                          0,   0, 255,
                        255, 255, 255};
    // clang-format on

    TextureData2d textureData(1, 4, 3, data);
    TextureFile::Write("test.tex", textureData);

    TextureData2d readTextureData2d = TextureFile::Read("test.tex");

    BOOST_CHECK(textureData.GetWidth() == readTextureData2d.GetWidth());
    BOOST_CHECK(textureData.GetHeight() == readTextureData2d.GetHeight());
    BOOST_CHECK(textureData.GetNumChannels() == readTextureData2d.GetNumChannels());
    BOOST_CHECK(textureData.GetPixelData().size() == readTextureData2d.GetPixelData().size());
    for (U32 i = 0; i < textureData.GetPixelData().size(); ++i)
        BOOST_CHECK(textureData.GetPixelData()[i] == readTextureData2d.GetPixelData()[i]);
}
