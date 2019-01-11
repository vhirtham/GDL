#include "gdl/input/keyMappingGLFW.h"

#include "gdl/base/exception.h"
#include "gdl/input/key.h"
#include <GLFW/glfw3.h>

#include <array>
#include <cstring>

namespace GDL::Input
{


auto GetMappingArray()
{
    std::array<Key, 1024> keyMapping;
    std::memset(&keyMapping, 0, sizeof(Key) * keyMapping.size());

    keyMapping[GLFW_KEY_ESCAPE] = Key::ESC;
    keyMapping[GLFW_KEY_A] = Key::A;
    keyMapping[GLFW_KEY_B] = Key::B;
    keyMapping[GLFW_KEY_C] = Key::C;
    keyMapping[GLFW_KEY_D] = Key::D;
    keyMapping[GLFW_KEY_E] = Key::E;
    keyMapping[GLFW_KEY_F] = Key::F;
    keyMapping[GLFW_KEY_G] = Key::G;
    keyMapping[GLFW_KEY_H] = Key::H;
    keyMapping[GLFW_KEY_I] = Key::I;
    keyMapping[GLFW_KEY_J] = Key::J;
    keyMapping[GLFW_KEY_K] = Key::K;
    keyMapping[GLFW_KEY_L] = Key::L;
    keyMapping[GLFW_KEY_M] = Key::M;
    keyMapping[GLFW_KEY_N] = Key::N;
    keyMapping[GLFW_KEY_O] = Key::O;
    keyMapping[GLFW_KEY_P] = Key::P;
    keyMapping[GLFW_KEY_Q] = Key::Q;
    keyMapping[GLFW_KEY_R] = Key::R;
    keyMapping[GLFW_KEY_S] = Key::S;
    keyMapping[GLFW_KEY_T] = Key::T;
    keyMapping[GLFW_KEY_U] = Key::U;
    keyMapping[GLFW_KEY_V] = Key::V;
    keyMapping[GLFW_KEY_W] = Key::W;
    keyMapping[GLFW_KEY_X] = Key::X;
    keyMapping[GLFW_KEY_Y] = Key::Y;
    keyMapping[GLFW_KEY_Z] = Key::Z;

    return keyMapping;
}


Key GLFWKeyToGDLKey(I32 glfwKey)
{
    static auto keyMapping = GetMappingArray();

    DEV_EXCEPTION(static_cast<U32>(glfwKey) >= keyMapping.size(), "Invalid key code");

    return keyMapping[static_cast<U32>(glfwKey)];
}

} // namespace GDL::Input
