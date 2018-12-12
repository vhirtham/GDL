#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/core/contextGLUT.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"


using namespace GDL;
using namespace GDL::OpenGL;


BOOST_AUTO_TEST_CASE(TestContextGLUT)
{
    ContextGLUT& contextGLUT = ContextGLUT::Instance();
    contextGLUT.EnableDebug();
    RenderWindowGLUT renderWindow(contextGLUT);
    renderWindow.Initialize();
}
