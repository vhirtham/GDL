#include <boost/test/unit_test.hpp>

#include "gdl/GDLTypedefs.h"
#include "gdl/rendering/openGL/renderWindowGL.h"
#include "gdl/rendering/openGL/uniformBlock.h"
#include "gdl/rendering/openGL/uniformBlockVariable.h"
#include "gdl/rendering/openGL/uniformBufferObject.h"

#include <map>
#include <string>
using namespace GDL;

RenderWindowGL& GetRenderWindow()
{
    static bool initialized = false;
    static RenderWindowGL renderWindow;
    if (!initialized)
    {
        renderWindow.Initialize();
        initialized = true;
    }
    return renderWindow;
}

BOOST_AUTO_TEST_CASE(Construct_From_UniformBlock)
{
    GetRenderWindow();

    constexpr U32 bufferSize = 160;
    std::vector<U8> buffer(bufferSize, 0);
    for (U32 i = 0; i < bufferSize; ++i)
        buffer[i] = i;

    std::map<std::string, UniformBlockVariable> uniformBlockVariables = {
            {"TranslationVec", UniformBlockVariable(0, 0, GL_FLOAT_VEC4, 1)},
            {"RotationMat", UniformBlockVariable(1, 16, GL_FLOAT_MAT4, 1)},
            {"OtherVec", UniformBlockVariable(2, 80, GL_FLOAT_VEC3, 1)},
            {"ProjectionMat", UniformBlockVariable(3, 96, GL_FLOAT_MAT4, 1)}};
    UniformBlock uniformBlock(1337, 2, bufferSize, uniformBlockVariables);

    UniformBufferObject uniformBufferObject_A(uniformBlock, GL_STATIC_DRAW);
    UniformBufferObject uniformBufferObject_B(uniformBlock, GL_STATIC_DRAW, std::move(buffer));

    int a = 0;
}
