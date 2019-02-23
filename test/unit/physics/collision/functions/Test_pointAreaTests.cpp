#include <boost/test/unit_test.hpp>

#include "gdl/math/constants.h"
#include "gdl/math/vec2.h"
#include "gdl/physics/collision/functions/pointAreaTests.h"

#include "test/tools/ExceptionChecks.h"


using namespace GDL;



// Point inside circle ------------------------------------------------------------------------------------------------


void TestPointInsideCircle(Vec2f c0, Vec2f c1, Vec2f c2)
{
    c0.Normalize();
    c1.Normalize();
    c2.Normalize();

    // Check Dev exception if points are not in counter clockwise order
    GDL_CHECK_THROW_DEV(PointInsideCircle(Vec2f(0, 0), c0, c2, c1), Exception);

    const U32 numAngles = 18;
    const F32 angleIncrement = 2.f * PI<F32> / numAngles;

    for (U32 scale = 1; scale < 10; ++scale)
    {
        Vec2f c0s = c0 * scale;
        Vec2f c1s = c1 * scale;
        Vec2f c2s = c2 * scale;

        for (I32 xOffset = -10; xOffset < 11; xOffset += 2)
            for (I32 yOffset = -10; yOffset < 11; yOffset += 2)
            {
                Vec2f offset(xOffset, yOffset);
                Vec2f c0so = c0s + offset;
                Vec2f c1so = c1s + offset;
                Vec2f c2so = c2s + offset;

                for (U32 i = 0; i < numAngles; ++i)
                {
                    F32 angle = i * angleIncrement;
                    Vec2f direction(std::cos(angle), std::sin(angle));


                    for (U32 j = 0; j < 10; ++j)
                    {
                        Vec2f point = offset + direction * j;

                        F32 result = PointInsideCircle(point, c0so, c1so, c2so);
                        if (scale == j)
                            // High tolerance since floating point errors add up significantly for some angles
                            BOOST_CHECK(result == ApproxZero<F32>(scale, 1e4f));
                        else if (j < scale)
                            BOOST_CHECK(result > 0);
                        else
                            BOOST_CHECK(result < 0);
                    }
                }
            }
    }
}


void TestPointInsideCircle()
{
}


BOOST_AUTO_TEST_CASE(Test_PointInsideCircle)
{
    TestPointInsideCircle({1, 0}, {0, 1}, {-1, 0});
    TestPointInsideCircle({3, 2}, {1, 1}, {-1, 5});
    TestPointInsideCircle({-7, 2}, {-4, -5}, {3, 8});
}
