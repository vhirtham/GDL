#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/core/renderWindowGL.h"

using namespace GDL;
using namespace GDL::OpenGL;

BOOST_AUTO_TEST_CASE(renderWindow)
{
    RenderWindow& renderWindow = RenderWindow::Instance();
    renderWindow.SetTitle("render window test");
    renderWindow.Initialize();
}
