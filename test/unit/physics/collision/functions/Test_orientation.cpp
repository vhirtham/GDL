#include <boost/test/unit_test.hpp>

#include "gdl/math/constants.h"
#include "gdl/math/mat3.h"
#include "gdl/math/transformations3.h"
#include "gdl/math/vec2.h"
#include "gdl/math/vec3.h"
#include "gdl/physics/collision/functions/orientation.h"

#include "test/tools/ExceptionChecks.h"

using namespace GDL;



// Orientation 2d -----------------------------------------------------------------------------------------------------

//! @brief This test creates several lines defined by two points a and b with slope angles from 0 to 350 degrees in 10
//! degree steps. Then it creates another point c and checks the result of the orientation function. If the point is
//! left of the line, than the result must be negative, positive otherwise. If c is on the line than the result must be
//! zero. Additionally the results absolute value must be twice the area of the triangle a-b-c.
BOOST_AUTO_TEST_CASE(Orientation_2D)
{
    Vec2fSSE a(3, -1);
    for (U32 i = 0; i < 36; ++i)
    {
        const F32 angleIncrement = 2.f * PI<F32> / 36.f;
        Vec2fSSE directionB(std::cos(i * angleIncrement), std::sin(i * angleIncrement));
        Vec2fSSE b = a + 3 * directionB;

        Vec2fSSE directionLeft(std::cos(i * angleIncrement + PI<F32> / 2.f),
                               std::sin(i * angleIncrement + PI<F32> / 2.f));

        // test for multiple c
        for (I32 j = -10; j < 11; ++j)
            for (I32 k = -10; k < 11; ++k)
            {
                Vec2fSSE c = a + static_cast<F32>(j) * directionB + static_cast<F32>(k) * directionLeft;
                F32 result = Orientation(a, b, c);

                // Check sign
                if (k < 0) // c is at the right hand side of the line a-b
                    BOOST_CHECK(result < 0);
                else if (k == 0) // c is on the line a-b
                    BOOST_CHECK(result == ApproxZero<F32>(1.f, 100));
                else // c is at the left hand side of the line a-b
                    BOOST_CHECK(result > 0);

                // Calculate twice of the triangles area
                if (k != 0)
                {
                    F32 lA = (c - b).Length();
                    F32 lB = (c - a).Length();
                    F32 lC = (b - a).Length();
                    // source: http://mathworld.wolfram.com/TriangleArea.html
                    F32 doubleArea =
                            0.5f * std::sqrt((lA + lB + lC) * (lB + lC - lA) * (lC + lA - lB) * (lA + lB - lC));

                    // The high tolerance (1000 *epsilon) is needed because the comparison formular has significant
                    // floating point errors
                    BOOST_CHECK(std::abs(result) == Approx(doubleArea, 1000));
                }
            }
    }
}



//! @brief Tests the sign of the 3d Orientation function. A plane is rotated using the provided rotation matrix. Several
//! points are tested against that rotated plane.
void TestOrientation3dSign(Mat3f rotation)
{
    Vec3fSSE<> a = rotation * Vec3fSSE<>{0, 1, 0};
    Vec3fSSE<> b = rotation * Vec3fSSE<>{-1, -1, 0};
    Vec3fSSE<> c = rotation * Vec3fSSE<>{1, -1, 0};
    Vec3fSSE<> orthogonal = rotation * Vec3fSSE<>{0, 0, 1};


    Vec3fSSE ba = b - a;
    Vec3fSSE ca = c - a;


    for (I32 i = -10; i < 11; ++i)
    {
        Vec3fSSE d = a + 0.5 * ba + 0.25 * ca + i * orthogonal;
        F32 result = Orientation(a, b, c, d);
        if (i < 0)
            BOOST_CHECK(result > 0);
        else if (i == 0)
            BOOST_CHECK(result == ApproxZero<F32>());
        else
            BOOST_CHECK(result < 0);
    }
}



// Orientation 3d -----------------------------------------------------------------------------------------------------

//! @brief Tests if the returnde volume of the 3d Orientation function equals 6 times the volume of the tetrahedron
//! described by the 4 passed points.
void TestOrientation3dValue(Vec3fSSE<> a, Vec3fSSE<> b, Vec3fSSE<> c, Vec3fSSE<> d, F32 expectedVolume, Mat3f rotation)
{
    a = rotation * a;
    b = rotation * b;
    c = rotation * c;
    d = rotation * d;

    for (U32 i = 1; i < 10; ++i)
        BOOST_CHECK(expectedVolume * 6 * i * i * i == Approx(std::abs(Orientation(a * i, b * i, c * i, d * i)), 10));
}



//! @brief Tests if the returnde volume of the 3d Orientation function by comparing it to the volume of multiple
//! tetrahedrons
void TestMultipleOrientation3dValues(Mat3f rotation)
{
    // regular Tetrahedrons
    TestOrientation3dValue({1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f, 1.f}, 2.f / 6.f, rotation);
    TestOrientation3dValue({1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 1.f, 1.f}, {0.f, 0.f, 1.f}, 2.f / 6.f, rotation);

    // arbitrary Tetrahedron
    // source: https://keisan.casio.com/exec/system/1223609147
    TestOrientation3dValue({0.f, 0.f, 0.f}, {2.f, 0.f, 0.f}, {0.f, 3.f, 0.f}, {1.f, 1.f, 2.f}, 2.f, rotation);
    TestOrientation3dValue({4.f, 1.f, 4.f}, {6.f, 2.f, 1.f}, {3.f, 1.f, 2.f}, {1.f, 3.f, 2.f}, 3.f, rotation);
}


BOOST_AUTO_TEST_CASE(Orientation_3D)
{
    constexpr F32 angleIncrement = 2.f * PI<F32> / 36.f;

    using namespace GDL::Transformations3;

    for (U32 i = 0; i < 36; ++i)
    {
        Mat3f rotZ = RotationZ(angleIncrement * i);
        for (U32 j = 0; j < 18; ++j)
        {
            Mat3f rotX = RotationX(angleIncrement * j);
            for (U32 k = 0; k < 18; ++k)
            {
                Mat3f rotY = RotationY(angleIncrement * k);
                Mat3f totalRotation = rotY * rotX * rotZ;
                TestOrientation3dSign(totalRotation);
                TestMultipleOrientation3dValues(totalRotation);
            }
        }
    }
}
