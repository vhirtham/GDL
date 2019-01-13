#include "gdl/input/keyMappingGLUT.h"

#include "gdl/base/exception.h"
#include "gdl/input/key.h"
#include <GL/freeglut.h>

#include <array>

namespace GDL::Input
{

constexpr auto GetMappingArray()
{
    std::array<Key, 1024> keyMapping = {};

    keyMapping[27] = Key::ESC;
    keyMapping['A'] = Key::A;
    keyMapping['B'] = Key::B;
    keyMapping['C'] = Key::C;
    keyMapping['D'] = Key::D;
    keyMapping['E'] = Key::E;
    keyMapping['F'] = Key::F;
    keyMapping['G'] = Key::G;
    keyMapping['H'] = Key::H;
    keyMapping['I'] = Key::I;
    keyMapping['J'] = Key::J;
    keyMapping['K'] = Key::K;
    keyMapping['L'] = Key::L;
    keyMapping['M'] = Key::M;
    keyMapping['N'] = Key::N;
    keyMapping['O'] = Key::O;
    keyMapping['P'] = Key::P;
    keyMapping['Q'] = Key::Q;
    keyMapping['R'] = Key::R;
    keyMapping['S'] = Key::S;
    keyMapping['T'] = Key::T;
    keyMapping['U'] = Key::U;
    keyMapping['V'] = Key::V;
    keyMapping['W'] = Key::W;
    keyMapping['X'] = Key::X;
    keyMapping['Y'] = Key::Y;
    keyMapping['Z'] = Key::Z;
    keyMapping['a'] = Key::A;
    keyMapping['b'] = Key::B;
    keyMapping['c'] = Key::C;
    keyMapping['d'] = Key::D;
    keyMapping['e'] = Key::E;
    keyMapping['f'] = Key::F;
    keyMapping['g'] = Key::G;
    keyMapping['h'] = Key::H;
    keyMapping['i'] = Key::I;
    keyMapping['j'] = Key::J;
    keyMapping['k'] = Key::K;
    keyMapping['l'] = Key::L;
    keyMapping['m'] = Key::M;
    keyMapping['n'] = Key::N;
    keyMapping['o'] = Key::O;
    keyMapping['p'] = Key::P;
    keyMapping['q'] = Key::Q;
    keyMapping['r'] = Key::R;
    keyMapping['s'] = Key::S;
    keyMapping['t'] = Key::T;
    keyMapping['u'] = Key::U;
    keyMapping['v'] = Key::V;
    keyMapping['w'] = Key::W;
    keyMapping['x'] = Key::X;
    keyMapping['y'] = Key::Y;
    keyMapping['z'] = Key::Z;

    return keyMapping;
}


Key GLUTKeyToGDLKey(I32 glutKey)
{
    constexpr auto keyMapping = GetMappingArray();
    DEV_EXCEPTION(static_cast<U32>(glutKey) >= keyMapping.size(), "Invalid key code");
    return keyMapping[static_cast<U32>(glutKey)];
}

} // namespace GDL::Input
