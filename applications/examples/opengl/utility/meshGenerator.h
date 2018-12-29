#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/vector.h"

#include <cmath>

namespace GDL::OpenGL::MeshGenerator
{
std::pair<Vector<F32>,Vector<U32>> CreateTorusMesh(F32 radiusMajor, F32 radiusMinor, U32 numMajorSegments, U32 numMinorSegments)
{
    const U32 valuesPerVertex = 6;
    const U32 indicesPerRectangularSegment = 6;
    const U32 numPoints = numMajorSegments * numMinorSegments;
    const U32 sizeVertexData = numPoints * valuesPerVertex;
    const U32 sizeIndexData = numPoints * indicesPerRectangularSegment;

    Vector<U32> indexData;
    Vector<F32> vertexData;
    vertexData.reserve(sizeVertexData);
    indexData.reserve(sizeIndexData);

    const F32 deltaAngleMajor = 2.f * static_cast<F32>(M_PI) / numMajorSegments;
    const F32 deltaAngleMinor = 2.f * static_cast<F32>(M_PI) / numMajorSegments;

    for (U32 i = 0; i < numMajorSegments; ++i)
    {
        F32 angleMajor = i * deltaAngleMajor;

        F32 xMajor = std::cos(angleMajor);
        F32 yMajor = std::sin(angleMajor);



        for (U32 j = 0; j < numMinorSegments; ++j)
        {
            F32 angleMinor = j * deltaAngleMinor;

            F32 xMinor = std::cos(angleMinor);
            F32 yMinor = std::sin(angleMinor);



            // Store vertex data
            F32 normalX = xMajor * xMinor;
            F32 normalY = yMajor * xMinor;
            F32 normalZ = yMinor;

            vertexData.push_back(normalX * radiusMinor + xMajor * radiusMajor);
            vertexData.push_back(normalY * radiusMinor + yMajor * radiusMajor);
            vertexData.push_back(normalZ * radiusMinor);
            vertexData.push_back(normalX);
            vertexData.push_back(normalY);
            vertexData.push_back(normalZ);

            // Store index data
            U32 segmentIndex1 = i * numMinorSegments;
            U32 segmentIndex2 = (i+1) * numMinorSegments;
            U32 j2 = j+1;

            if(segmentIndex2 >= numPoints)
                segmentIndex2 = 0;
            if(j2==numMinorSegments)
                j2 = 0;

            indexData.push_back(segmentIndex1 + j);
            indexData.push_back(segmentIndex1 + j2);
            indexData.push_back(segmentIndex2 + j2);
            indexData.push_back(segmentIndex2 + j2);
            indexData.push_back(segmentIndex2 + j);
            indexData.push_back(segmentIndex1 + j);
        }
    }

    assert(sizeVertexData == vertexData.size());
    assert(sizeIndexData == indexData.size());

    return std::make_pair(vertexData,indexData);
}
} // namespace GDL::OpenGL::MeshGenerator
