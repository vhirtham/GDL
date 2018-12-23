#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/core/contextManagerGLFW.h"
#include "gdl/rendering/openGL/core/renderWindowGLFW.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;
using namespace GDL::OpenGL;


BOOST_AUTO_TEST_CASE(TestContextManagerGLFW)
{
    ContextManagerGLFW& contextManagerGLFW = ContextManagerGLFW::Instance();
    contextManagerGLFW.EnableDebug();
    contextManagerGLFW.Initialize();

    RenderWindowGLFW renderWindow(contextManagerGLFW);
    renderWindow.Initialize();
}
