#include <boost/test/unit_test.hpp>

#include "gdl/GDLTypedefs.h"
#include "gdl/rendering/openGL/core/renderWindowGL.h"
#include "gdl/rendering/openGL/core/uniformBlock.h"
#include "gdl/rendering/openGL/core/uniformBlockVariable.h"
#include "gdl/rendering/openGL/core/uniformBufferObject.h"

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
    std::vector<U8> bufferMismatched(bufferSize + 1, 0);

    std::map<std::string, UniformBlockVariable> uBVars = {
            {"TranslationVec", UniformBlockVariable(0, 0, GL_FLOAT_VEC4, 1)},
            {"RotationMat", UniformBlockVariable(1, 16, GL_FLOAT_MAT4, 1)},
            {"OtherVec", UniformBlockVariable(2, 80, GL_FLOAT_VEC3, 1)},
            {"ProjectionMat", UniformBlockVariable(3, 96, GL_FLOAT_MAT4, 1)}};
    UniformBlock uniformBlock(1337, 2, bufferSize, uBVars);

    UniformBufferObject uniformBufferObject_A(uniformBlock, GL_STATIC_DRAW);
    UniformBufferObject uniformBufferObject_B(uniformBlock, GL_STATIC_DRAW, buffer);


    BOOST_CHECK(uniformBufferObject_A.CheckUniformBlockCompatibility(uniformBlock));
    BOOST_CHECK(uniformBufferObject_B.CheckUniformBlockCompatibility(uniformBlock));

    BOOST_CHECK(uniformBufferObject_A.GetBindingPoint() == uniformBlock.GetBindingPoint());
    BOOST_CHECK(uniformBufferObject_B.GetBindingPoint() == uniformBlock.GetBindingPoint());

    BOOST_CHECK_THROW(UniformBufferObject(uniformBlock, GL_STATIC_DRAW, bufferMismatched), Exception);
}


BOOST_AUTO_TEST_CASE(UniformBlockCompatibility)
{
    GetRenderWindow();

    constexpr U32 bufferSize = 160;

    std::map<std::string, UniformBlockVariable> uBVars = {
            {"TranslationVec", UniformBlockVariable(0, 0, GL_FLOAT_VEC4, 1)},
            {"RotationMat", UniformBlockVariable(1, 16, GL_FLOAT_MAT4, 1)},
            {"OtherVec", UniformBlockVariable(2, 80, GL_FLOAT_VEC3, 1)}};
    std::map<std::string, UniformBlockVariable> uBVarsDiffMemberNames(uBVars);
    std::map<std::string, UniformBlockVariable> uBVarsDiffMemberDataAllowed(uBVars);
    std::map<std::string, UniformBlockVariable> uBVarsDiffMemberDataForbidden(uBVars);
    uBVars.emplace("ProjectionMat", UniformBlockVariable(3, 96, GL_FLOAT_MAT4, 1));
    uBVarsDiffMemberNames.emplace("ProTectionMat", UniformBlockVariable(3, 96, GL_FLOAT_MAT4, 1));
    uBVarsDiffMemberDataAllowed.emplace("ProjectionMat", UniformBlockVariable(555, 96, GL_FLOAT_MAT4, 1));
    uBVarsDiffMemberDataForbidden.emplace("ProjectionMat", UniformBlockVariable(3, 96, GL_FLOAT_MAT3, 1));
    std::map<std::string, UniformBlockVariable> uBVarsDiffNumber(uBVars);
    uBVarsDiffNumber.emplace("ExtraMat", UniformBlockVariable(3, 96, GL_FLOAT_MAT4, 1));


    UniformBlock uniformBlockRef(1337, 2, bufferSize, uBVars);
    UniformBlock uniformBlockDiffIndex(42, 2, bufferSize, uBVars);
    UniformBlock uniformBlockDiffBinding(1337, 3, bufferSize, uBVars);
    UniformBlock uniformBlockDiffSize(1337, 2, bufferSize + 1, uBVars);
    UniformBlock uniformBlockDiffNumVariables(1337, 2, bufferSize, uBVarsDiffNumber);
    UniformBlock uniformBlockDiffMemberNames(1337, 2, bufferSize, uBVarsDiffMemberNames);
    UniformBlock uniformBlockDiffMemberDataAllowed(1337, 2, bufferSize, uBVarsDiffMemberDataAllowed);
    UniformBlock uniformBlockDiffMemberDataForbidden(1337, 2, bufferSize, uBVarsDiffMemberDataForbidden);

    UniformBufferObject uniformBufferObject(uniformBlockRef, GL_STATIC_DRAW);

    // Only data structure related mismatches should return false!
    BOOST_CHECK(uniformBufferObject.CheckUniformBlockCompatibility(uniformBlockRef));
    BOOST_CHECK(uniformBufferObject.CheckUniformBlockCompatibility(uniformBlockDiffIndex));
    BOOST_CHECK(uniformBufferObject.CheckUniformBlockCompatibility(uniformBlockDiffBinding));
    BOOST_CHECK(uniformBufferObject.CheckUniformBlockCompatibility(uniformBlockDiffSize) == false);
    BOOST_CHECK(uniformBufferObject.CheckUniformBlockCompatibility(uniformBlockDiffNumVariables) == false);
    BOOST_CHECK(uniformBufferObject.CheckUniformBlockCompatibility(uniformBlockDiffMemberNames) == false);
    BOOST_CHECK(uniformBufferObject.CheckUniformBlockCompatibility(uniformBlockDiffMemberDataAllowed));
    BOOST_CHECK(uniformBufferObject.CheckUniformBlockCompatibility(uniformBlockDiffMemberDataForbidden) == false);
}
