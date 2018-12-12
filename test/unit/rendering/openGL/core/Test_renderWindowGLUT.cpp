#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/core/contextGLUT.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"

using namespace GDL;
using namespace GDL::OpenGL;

// TODO: Add some real test cases to this test

BOOST_AUTO_TEST_CASE(renderWindow)
{
    ContextGLUT& contextGLUT = ContextGLUT::Instance();
    contextGLUT.EnableDebug();
    RenderWindowGLUT renderWindow(contextGLUT);
    renderWindow.SetTitle("render window test");
    renderWindow.Initialize();

    renderWindow.GetHeight();
    renderWindow.GetWidth();
    renderWindow.IsInitialized();
}
