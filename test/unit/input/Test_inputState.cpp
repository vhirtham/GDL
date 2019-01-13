#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/input/inputState.h"
#include "gdl/input/key.h"

#include "test/tools/ExceptionChecks.h"

using namespace GDL;
using namespace GDL::Input;


BOOST_AUTO_TEST_CASE(Test_Keyboard_State)
{
    constexpr Key modKey = Key::E;

    // Every state is initialized to false
    for (U32 i = 0; i < 512; ++i)
        BOOST_CHECK(InputState::GetKeyPressed(static_cast<Key>(i)) == false);

    InputState::SetKeyPressed(modKey, true);
    BOOST_CHECK(InputState::GetKeyPressed(modKey) == true);

    GDL_CHECK_THROW_DEV_DISABLE(InputState::SetKeyPressed(static_cast<Key>(10000), true), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(InputState::GetKeyPressed(static_cast<Key>(10000)), Exception);
}

BOOST_AUTO_TEST_CASE(Test_Mouse_State)
{
    constexpr F64 mouseX = 15;
    constexpr F64 mouseY = 405;

    InputState::SetMousePosition(mouseX, mouseY);
    BOOST_CHECK(InputState::GetMousePositionX() == Approx(mouseX));
    BOOST_CHECK(InputState::GetMousePositionY() == Approx(mouseY));
}
