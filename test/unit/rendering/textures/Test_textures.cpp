#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/rendering/textures/textureData.h"
#include "gdl/rendering/textures/textureFile.h"

#include "test/tools/ExceptionChecks.h"


using namespace GDL;



BOOST_AUTO_TEST_CASE(Texture_Data)
{
    // clang-format off
    Vector<U8> data = { 255,   0,   0,
                          0, 255,   0,
                          0,   0, 255,
                        255, 255, 255};
    // clang-format on

    TextureData textureData(2, 2, 3, data);

    GDL_CHECK_THROW_DEV_DISABLE(TextureData(0, 4, 3, data), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(TextureData(4, 0, 3, data), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(TextureData(2, 6, 0, data), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(TextureData(2, 3, 3, data), Exception);
}



BOOST_AUTO_TEST_CASE(Texture_File)
{
    // clang-format off
    Vector<U8> data = { 255,   0,   0,
                          0, 255,   0,
                          0,   0, 255,
                        255, 255, 255};
    // clang-format on

    TextureData textureData(1, 4, 3, data);
    TextureFile::Write("test.tex", textureData);

    TextureData readTextureData = TextureFile::Read("test.tex");

    BOOST_CHECK(textureData.GetWidth() == readTextureData.GetWidth());
    BOOST_CHECK(textureData.GetHeight() == readTextureData.GetHeight());
    BOOST_CHECK(textureData.GetNumChannels() == readTextureData.GetNumChannels());
    BOOST_CHECK(textureData.GetPixelData().size() == readTextureData.GetPixelData().size());
    for (U32 i = 0; i < textureData.GetPixelData().size(); ++i)
        BOOST_CHECK(textureData.GetPixelData()[i] == readTextureData.GetPixelData()[i]);
}
