#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/core/renderWindowGL.h"

using namespace GDL;

BOOST_AUTO_TEST_CASE(renderWindow)
{
    RenderWindowGL renderWindow;
    renderWindow.SetTitle("render window test");
    renderWindow.Initialize();
}
