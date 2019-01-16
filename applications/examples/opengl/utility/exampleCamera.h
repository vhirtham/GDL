#pragma once



#include "gdl/math/mat4.h"
#include "gdl/math/vec4.h"

namespace GDL
{


namespace OpenGL
{

//! @brief Camera class for examples. W,A,S,D-keys move the camera and the mouse movements control the view direction
class ExampleCamera
{

    Vec4f mPosition;
    F32 mPitch;
    F32 mYaw;
    F32 mMouseSensitivity = 1.f / 100.f;
    F32 mMovementSpeed = 1.f;


public:
    ExampleCamera() = delete;
    ExampleCamera(const ExampleCamera&) = delete;
    ExampleCamera(ExampleCamera&&) = delete;
    ExampleCamera& operator=(const ExampleCamera&) = delete;
    ExampleCamera& operator=(ExampleCamera&&) = delete;
    ~ExampleCamera() = default;

    //! @brief Ctor
    //! @param x: X-position
    //! @param y: Y-position
    //! @param z: Z-position
    ExampleCamera(F32 x, F32 y, F32 z, F32 yaw = 0.f, F32 pitch = 0.f);

    //! @brief Calculates and returns the world-to-camera-space matrix
    //! @return World-to-camera-space matrix
    Mat4f GetWorldToCamMatrix() const;

    //! @brief Gets the cameras position
    //! @return Camera position
    const Vec4f& GetCameraPosition() const;

    //! @brief Gets the cameras position
    //! @return Camera position
    Vec4f GetCameraDirection() const;

    //! @brief Calculates the new camera position
    //! @param deltaTime: Passed time in seconds
    void MoveCamera(F32 deltaTime);

    //! @brief Sets the cameras movement speed
    //! @param movementSpeed: New movement speed
    void SetMovementSpeed(F32 movementSpeed);

private:
    //! @brief Gets the movement direction vector depending on the input state
    //! @return Movement direction vector
    static Vec4f GetMovementDirection();

    //! @brief Gets the change of the mouse position depending on the input state
    //! @return Change of the mouse position
    static std::array<F32, 2> GetMouseDelta();

    //! @param Sets the camera position
    //! @param x: X-position
    //! @param y: Y-position
    //! @param z: Z-position
    void SetPosition(F32 x, F32 y, F32 z);
};

} // namespace OpenGL
} // namespace GDL

#include "applications/examples/opengl/utility/exampleCamera.inl"
