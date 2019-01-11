#include "gdl/input/inputState.h"



#include "gdl/base/exception.h"
#include "gdl/input/key.h"


#include <cstring>


namespace GDL::Input
{

InputState::InputState()
{
    std::memset(&mKeyPressed, 0, sizeof(bool) * mKeyPressed.size());
}



bool InputState::GetKeyPressed(Key key)
{
    InputState& inputState = Instance();
    return inputState.mKeyPressed[static_cast<U32>(key)];
}



InputState& InputState::Instance()
{
    static InputState inputState;
    return inputState;
}



void InputState::SetKeyPressed(Key key, bool keyPressed)
{
    InputState& inputState = Instance();
    DEV_EXCEPTION(static_cast<U32>(key) >= inputState.mKeyPressed.size(), "Invalid key code");
    inputState.mKeyPressed[static_cast<U32>(key)] = keyPressed;
}


} // namespace GDL::Input
