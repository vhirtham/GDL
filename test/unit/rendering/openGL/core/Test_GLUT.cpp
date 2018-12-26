#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/core/contextManagerGLUT.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"

#include "test/unit/rendering/openGL/core/ContextManagerRenderWindowTests.h"



using namespace GDL;
using namespace GDL::OpenGL;


BOOST_AUTO_TEST_CASE(Test_ContextManagerAndRenderWindow)
{
    TestContextManagerAndRenderWindow<ContextManagerGLUT, RenderWindowGLUT>();
}



BOOST_AUTO_TEST_CASE(Test_DebugMessageHandler)
{
    TestDebugMessageHandler<ContextManagerGLUT>();
}
