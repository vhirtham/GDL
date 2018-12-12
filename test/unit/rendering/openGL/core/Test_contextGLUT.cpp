#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/rendering/openGL/core/contextGLUT.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;
using namespace GDL::OpenGL;


BOOST_AUTO_TEST_CASE(TestContextGLUT)
{
    ContextGLUT& contextGLUT = ContextGLUT::Instance();
    RenderWindowGLUT renderWindow(contextGLUT);
}
