#include <boost/test/unit_test.hpp>

#include "gdl/base/Exception.h"
#include "gdl/rendering/openGL/renderWindowGL.h"
#include "gdl/rendering/openGL/shaderGL.h"

using namespace GDL;

BOOST_AUTO_TEST_CASE(renderWindow)
{
    RenderWindowGL renderWindow;
    renderWindow.Initialize();

    BOOST_CHECK_THROW(ShaderGL shader(GL_VERTEX_SHADER, "fail"), Exception);
}
