#include <boost/test/unit_test.hpp>

#include "gdl/math/constants.h"
#include "gdl/math/vec2.h"
#include "gdl/physics/collision/functions/orientation.h"

#include "test/tools/ExceptionChecks.h"

using namespace GDL;


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
