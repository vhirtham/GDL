#include "gdl/input/inputState.h"



#include "gdl/base/exception.h"
#include "gdl/input/key.h"



namespace GDL::Input
{

InputState::InputState()
    : mKeyPressed{{0}}
{
}



bool InputState::GetKeyPressed(Key key)
{
    InputState& inputState = Instance();
    DEV_EXCEPTION(static_cast<U32>(key) >= inputState.mKeyPressed.size(), "Invalid key code");
    return inputState.mKeyPressed[static_cast<U32>(key)];
}



F64 InputState::GetMousePositionX()
{
    return Instance().mMouseX;
}



F64 InputState::GetMousePositionY()
{
    return Instance().mMouseY;
}



void InputState::SetKeyPressed(Key key, bool keyPressed)
{
    InputState& inputState = Instance();
    DEV_EXCEPTION(static_cast<U32>(key) >= inputState.mKeyPressed.size(), "Invalid key code");
    inputState.mKeyPressed[static_cast<U32>(key)] = keyPressed;
}



void InputState::SetMousePosition(F64 mouseX, F64 mouseY)
{
    InputState& inputState = Instance();
    inputState.mMouseX = mouseX;
    inputState.mMouseY = mouseY;
}



InputState& InputState::Instance()
{
    static InputState inputState;
    return inputState;
}
} // namespace GDL::Input
