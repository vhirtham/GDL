#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/core/contextManagerGLFW.h"
#include "gdl/rendering/openGL/core/renderWindowGLFW.h"

#include "test/unit/rendering/openGL/core/ContextManagerRenderWindowTests.h"



using namespace GDL;
using namespace GDL::OpenGL;


BOOST_AUTO_TEST_CASE(Test_ContextManagerAndRenderWindow)
{
    TestContextManagerAndRenderWindow<ContextManagerGLFW, RenderWindowGLFW>();
}



BOOST_AUTO_TEST_CASE(Test_DebugMessageHandler)
{
    TestDebugMessageHandler<ContextManagerGLFW>();
}
