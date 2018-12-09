#include <boost/test/unit_test.hpp>


#include "gdl/base/fundamentalTypes.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"
#include "gdl/rendering/openGL/core/uniformBufferObjectGL.h"
#include "gdl/rendering/openGL/management/uniformBlockGL.h"
#include "gdl/rendering/openGL/management/uniformBlockVariableGL.h"
#include "gdl/rendering/openGL/management/managedUniformBufferObjectGL.h"

#include <map>
#include <string>
using namespace GDL;
using namespace GDL::OpenGL;

RenderWindowGLUT& GetRenderWindowGLUT()
{
    static bool initialized = false;
    RenderWindowGLUT& renderWindow = RenderWindowGLUT::Instance();
    if (!initialized)
    {
        renderWindow.Initialize();
        initialized = true;
    }
    return renderWindow;
}

BOOST_AUTO_TEST_CASE(Construct_From_UniformBlock)
{
    GetRenderWindowGLUT();

    constexpr U32 bufferSize = 160;
    std::vector<U8> buffer(bufferSize, 0);
    std::vector<U8> bufferMismatched(bufferSize + 1, 0);

    std::map<std::string, UniformBlockVariable> uBVars = {
            {"TranslationVec", UniformBlockVariable(0, 0, GL_FLOAT_VEC4, 1)},
            {"RotationMat", UniformBlockVariable(1, 16, GL_FLOAT_MAT4, 1)},
            {"OtherVec", UniformBlockVariable(2, 80, GL_FLOAT_VEC3, 1)},
            {"ProjectionMat", UniformBlockVariable(3, 96, GL_FLOAT_MAT4, 1)}};
    UniformBlock uniformBlock(1337, 2, bufferSize, uBVars);

    ManagedUniformBufferObject managedUBO_A(uniformBlock, GL_STATIC_DRAW);
    ManagedUniformBufferObject managedUBO_B(uniformBlock, GL_STATIC_DRAW, buffer);


    BOOST_CHECK(managedUBO_A.CheckUniformBlockCompatibility(uniformBlock));
    BOOST_CHECK(managedUBO_B.CheckUniformBlockCompatibility(uniformBlock));

    BOOST_CHECK(managedUBO_A.GetBindingPoint() == uniformBlock.GetBindingPoint());
    BOOST_CHECK(managedUBO_B.GetBindingPoint() == uniformBlock.GetBindingPoint());

    BOOST_CHECK_THROW(ManagedUniformBufferObject(uniformBlock, GL_STATIC_DRAW, bufferMismatched), Exception);
}

BOOST_AUTO_TEST_CASE(UniformBlock_Compatibility)
{
    GetRenderWindowGLUT();

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

    ManagedUniformBufferObject managedUBO(uniformBlockRef, GL_STATIC_DRAW);

    // Only data structure related mismatches should return false!
    BOOST_CHECK(managedUBO.CheckUniformBlockCompatibility(uniformBlockRef));
    BOOST_CHECK(managedUBO.CheckUniformBlockCompatibility(uniformBlockDiffIndex));
    BOOST_CHECK(managedUBO.CheckUniformBlockCompatibility(uniformBlockDiffBinding));
    BOOST_CHECK(managedUBO.CheckUniformBlockCompatibility(uniformBlockDiffSize) == false);
    BOOST_CHECK(managedUBO.CheckUniformBlockCompatibility(uniformBlockDiffNumVariables) == false);
    BOOST_CHECK(managedUBO.CheckUniformBlockCompatibility(uniformBlockDiffMemberNames) == false);
    BOOST_CHECK(managedUBO.CheckUniformBlockCompatibility(uniformBlockDiffMemberDataAllowed));
    BOOST_CHECK(managedUBO.CheckUniformBlockCompatibility(uniformBlockDiffMemberDataForbidden) == false);
}
