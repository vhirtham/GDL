#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/rendering/openGL/core/texture.h"
#include "gdl/rendering/textures/textureData.h"

#include "test/tools/GetRenderWindow.h"

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

    TextureData textureData(width, height, numChannels, data);

    Texture texture(textureData);
}
