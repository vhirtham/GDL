#pragma once

#include "gdl/base/fundamentalTypes.h"


#include <array>

namespace GDL::Input
{

enum class Key;

//! @brief Singleton class that stores the current input state.
class InputState
{

    std::array<bool,1024> mKeyPressed;


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


    //! @brief Sets a key state
    //! @param key: Key code
    //! @param keyPressed: New state of the key
    static void SetKeyPressed(Key key, bool keyPressed);

private:
    //! @brief Gets the singleton instance
    //! @return Singleton instance
    static InputState& Instance();
};

} // namespace GDL::Input
