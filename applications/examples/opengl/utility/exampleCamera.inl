#pragma once

#include "applications/examples/opengl/utility/exampleCamera.h"

#include "gdl/math/constants.h"
#include "gdl/math/transformationMatrix.h"
#include "gdl/input/inputState.h"
#include "gdl/input/key.h"


#include <iostream>

namespace GDL::OpenGL
{



ExampleCamera::ExampleCamera(F32 x, F32 y, F32 z, F32 yaw, F32 pitch)
    : mPosition{x, y, z, 1.f}
    , mPitch{pitch}
    , mYaw{yaw}
{
}

void ExampleCamera::MoveCamera(F32 deltaTime)
{
    std::array<F32, 2> mouseDelta = GetMouseDelta();

    mYaw += mouseDelta[0] * mMouseSensitivity;
    mPitch += mouseDelta[1] * mMouseSensitivity;
    if (mPitch > PI<F32> / 2.f)
        mPitch = PI<F32> / 2.f;
    if (mPitch < -PI<F32> / 2.f)
        mPitch = -PI<F32> / 2.f;



    mPosition += TransformationMatrix4::RotationZ(-mYaw) * GetMovementDirection() * deltaTime;
}



Mat4f ExampleCamera::GetWorldToCamMatrix() const
{
    std::array<F32, 4> translation = mPosition.Data();
    return TransformationMatrix4::RotationX(mPitch) * TransformationMatrix4::RotationY(mYaw) *
           TransformationMatrix4::RotationX(-PI<F32> / 2.f) *
           TransformationMatrix4::Translation(-translation[0], -translation[1], -translation[2]);
}



Vec4f ExampleCamera::GetMovementDirection()
{
    using namespace GDL::Input;

    F32 moveX = 0;
    F32 moveY = 0;


    if (InputState::GetKeyPressed(Key::W))
        moveY += 1;
    if (InputState::GetKeyPressed(Key::S))
        moveY -= 1;
    if (InputState::GetKeyPressed(Key::D))
        moveX += 1;
    if (InputState::GetKeyPressed(Key::A))
        moveX -= 1;

    Vec4f movementDirection(moveX, moveY, 0, 0);
    if (std::abs(moveX) + std::abs(moveY) > 0)
        movementDirection.Normalize();

    return movementDirection;
}



std::array<F32, 2> ExampleCamera::GetMouseDelta()
{
    using namespace GDL::Input;

    static std::array<F32, 2> prevMousePos = {
            {static_cast<F32>(InputState::GetMousePositionX()), static_cast<F32>(InputState::GetMousePositionY())}};

    std::array<F32, 2> mouseDelta = {{static_cast<F32>(InputState::GetMousePositionX()) - prevMousePos[0],
                                      static_cast<F32>(InputState::GetMousePositionY()) - prevMousePos[1]}};

    prevMousePos[0] = static_cast<F32>(InputState::GetMousePositionX());
    prevMousePos[1] = static_cast<F32>(InputState::GetMousePositionY());

    return mouseDelta;
}

void ExampleCamera::SetPosition(F32 x, F32 y, F32 z)
{
    mPosition = Vec4f(x, y, z, 1.f);
}

} // namespace GDL::OpenGL
