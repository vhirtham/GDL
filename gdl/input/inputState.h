#pragma once

#include "gdl/base/fundamentalTypes.h"


#include <array>

namespace GDL::Input
{

enum class Key;

//! @brief Singleton class that stores the current input state.
class InputState
{

    std::array<bool, 512> mKeyPressed;
    F64 mMouseX = -1;
    F64 mMouseY = -1;

    //! @brief Private ctor
    InputState();

public:
    InputState(const InputState&) = delete;
    InputState(InputState&&) = delete;
    InputState& operator=(const InputState&) = delete;
    InputState& operator=(InputState&&) = delete;
    ~InputState() = default;


    //! @brief Gets the state of a key
    //! @param key: Key code
    //! @return True if the key is pressed, false otherwise
    static bool GetKeyPressed(Key key);

    //! @brief Gets the mouse x position
    //! @return Mouse x position
    static F64 GetMousePositionX();

    //! @brief Gets the mouse y position
    //! @return Mouse y position
    static F64 GetMousePositionY();

    //! @brief Sets a key state
    //! @param key: Key code
    //! @param keyPressed: New state of the key
    static void SetKeyPressed(Key key, bool keyPressed);

    //! @brief Sets the mouse position
    //! @param mouseX: Mouse x position
    //! @param mouseY: Mouse y position
    static void SetMousePosition(F64 mouseX, F64 mouseY);

private:
    //! @brief Gets the singleton instance
    //! @return Singleton instance
    static InputState& Instance();
};

} // namespace GDL::Input
