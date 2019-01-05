#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/base/functions/pow.h"
#include "gdl/rendering/openGL/core/texture.h"
#include "gdl/rendering/textures/textureData2d.h"

#include "test/tools/GetRenderWindow.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;
using namespace GDL::OpenGL;

BOOST_AUTO_TEST_CASE(Test_Texture2d)
{
    GetRenderWindow();


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

    Texture texture(textureData, 3);
    texture.CreateMipMaps();
    texture.SetWrappingXY(GL_CLAMP_TO_BORDER);
    texture.SetBorderColor({{1.f, 0.f, 1.f, 1.f}});
    texture.SetMinifyingFilter(GL_LINEAR);
    texture.SetMagnifyingFilter(GL_LINEAR);

    texture.Bind(0);
    // Too many texture levels
    GDL_CHECK_THROW_DEV_DISABLE(Texture(textureData, 5), Exception);
}


BOOST_AUTO_TEST_CASE(MaxNumLevels)
{
    for (U32 i = 1; i < 10; ++i)
    {
        U32 value = Pow<U32>(2, static_cast<I32>(i));
        BOOST_CHECK(Texture::GetMaxNumTextureLevels2d(value, 1) == i + 1);
        BOOST_CHECK(Texture::GetMaxNumTextureLevels2d(value - 1, 1) == i);
        BOOST_CHECK(Texture::GetMaxNumTextureLevels2d(1, value) == i + 1);
        BOOST_CHECK(Texture::GetMaxNumTextureLevels2d(1, value - 1) == i);
        BOOST_CHECK(Texture::GetMaxNumTextureLevels2d(value, value) == i + 1);
        BOOST_CHECK(Texture::GetMaxNumTextureLevels2d(value - 1, value - 1) == i);
    }
}
