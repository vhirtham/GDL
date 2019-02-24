#include <boost/test/unit_test.hpp>

#include "gdl/math/constants.h"
#include "gdl/math/transformations3.h"
#include "gdl/math/vec3.h"
#include "gdl/physics/collision/functions/pointVolumeTests.h"

#include "test/tools/ExceptionChecks.h"


using namespace GDL;
using namespace GDL::Transformations3;



// Point inside sphere ------------------------------------------------------------------------------------------------


void TestPointInsideSphere(Vec3f c0, Vec3f c1, Vec3f c2, Vec3f c3)
{
    c0.Normalize();
    c1.Normalize();
    c2.Normalize();
    c3.Normalize();

    // Check Dev exception if points are not in counter clockwise order
    GDL_CHECK_THROW_DEV(PointInsideSphere(Vec3f(0, 0, 0), c0, c2, c1, c3), Exception);

    const U32 numAnglesY = 7;
    const U32 numAnglesZ = 10;
    const F32 angleIncrementY = 2.f * PI<F32> / (numAnglesY - 1);
    const F32 angleIncrementZ = 2.f * PI<F32> / numAnglesZ;

    for (U32 scale = 1; scale < 10; scale += 3)
    {
        Vec3f c0s = c0 * scale;
        Vec3f c1s = c1 * scale;
        Vec3f c2s = c2 * scale;
        Vec3f c3s = c3 * scale;

        for (I32 xOffset = -10; xOffset < 11; xOffset += 5)
            for (I32 yOffset = -10; yOffset < 11; yOffset += 5)
                for (I32 zOffset = -10; zOffset < 11; zOffset += 5)
                {
                    Vec3f offset(xOffset, yOffset, zOffset);
                    Vec3f c0so = c0s + offset;
                    Vec3f c1so = c1s + offset;
                    Vec3f c2so = c2s + offset;
                    Vec3f c3so = c3s + offset;

                    for (U32 i = 0; i < numAnglesY; ++i)
                        for (U32 j = 0; j < numAnglesZ; ++j)
                        {
                            F32 angleY = i * angleIncrementY - PI<F32> / 2.f;
                            F32 angleZ = j * angleIncrementZ;
                            Vec3f direction = RotationZ(angleZ) * RotationY(angleY) * Vec3f(1, 0, 0);
                            for (U32 k = 0; k < 10; ++k)
                            {
                                Vec3f point = offset + direction * k;

                                F32 result = PointInsideSphere(point, c0so, c1so, c2so, c3so);
                                if (scale == k)
                                    // High tolerance since floating point errors add up significantly for some angles
                                    BOOST_CHECK(result == ApproxZero<F32>(scale, 1e5f));
                                else if (k < scale)
                                    BOOST_CHECK(result > 0);
                                else
                                    BOOST_CHECK(result < 0);
                            }
                        }
                }
    }
}



BOOST_AUTO_TEST_CASE(Test_PointInsideSphere)
{
    TestPointInsideSphere({-1, 0, 0}, {0, 1, 0}, {1, 0, 0}, {0, 0, 1});
    TestPointInsideSphere({2, 3, 4}, {-3, 7, 1}, {2, -2, 8}, {-4, -6, -2});
}
