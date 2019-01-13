#pragma once

#include "applications/examples/opengl/utility/meshGenerator.h"

#include <cassert>
#include <cmath>

namespace GDL::OpenGL::MeshGenerator
{

template <bool _addNormals, bool _addTexCoordinates>
std::pair<Vector<F32>, Vector<U32>> CreateRectangle(F32 width, F32 height)
{
    constexpr U32 numPositionValuesPerVertex = 3;
    constexpr U32 numNormalValuesPerVertex = _addNormals ? 3 : 0;
    constexpr U32 numTexCoordinatesPerVertex = _addTexCoordinates ? 2 : 0;

    constexpr U32 numValuesPerVertex =
            numPositionValuesPerVertex + numNormalValuesPerVertex + numTexCoordinatesPerVertex;

    constexpr U32 sizeVertexData = numValuesPerVertex * 4;

    Vector<U32> elementData = {{0, 1, 2, 2, 1, 3}};
    Vector<F32> vertexData;
    vertexData.reserve(sizeVertexData);

    for (U32 i = 0; i < 2; ++i)
        for (U32 j = 0; j < 2; ++j)
        {
            vertexData.push_back((static_cast<F32>(i) - 0.5f) * width);
            vertexData.push_back((static_cast<F32>(j) - 0.5f) * height);
            vertexData.push_back(0.f);
            if constexpr (_addNormals == true)
            {
                vertexData.push_back(0.f);
                vertexData.push_back(0.f);
                vertexData.push_back(1.f);
            }

            if constexpr (_addTexCoordinates == true)
            {
                vertexData.push_back(static_cast<F32>(i));
                vertexData.push_back(static_cast<F32>(j));
            }
        }

    assert(sizeVertexData == vertexData.size());
    assert(6 == elementData.size());


    return std::make_pair(vertexData, elementData);
}




template <bool _addNormals, bool _addTexCoordinates>
std::pair<Vector<F32>, Vector<U32>> CreateTorus(F32 radiusMajor, F32 radiusMinor, U32 numMajorSegments,
                                                U32 numMinorSegments)
{
    constexpr U32 numPositionValuesPerVertex = 3;
    constexpr U32 numNormalValuesPerVertex = _addNormals ? 3 : 0;
    constexpr U32 numTexCoordinatesPerVertex = _addTexCoordinates ? 2 : 0;
    const U32 valuesPerVertex = numPositionValuesPerVertex + numNormalValuesPerVertex + numTexCoordinatesPerVertex;

    const U32 indicesPerRectangularSegment = 6;
    const U32 numPoints = (numMajorSegments + 1) * (numMinorSegments + 1);
    const U32 sizeVertexData = numPoints * valuesPerVertex;
    const U32 sizeIndexData = numMajorSegments * numMinorSegments * indicesPerRectangularSegment;

    Vector<U32> indexData;
    Vector<F32> vertexData;
    vertexData.reserve(sizeVertexData);
    indexData.reserve(sizeIndexData);

    const F32 deltaAngleMajor = 2.f * static_cast<F32>(M_PI) / numMajorSegments;
    const F32 deltaAngleMinor = 2.f * static_cast<F32>(M_PI) / numMajorSegments;
    [[maybe_unused]] const F32 deltaTexCoordX = 1.f / numMajorSegments;
    [[maybe_unused]] const F32 deltaTexCoordY = 1.f / numMinorSegments;

    for (U32 i = 0; i < numMajorSegments + 1; ++i)
    {
        F32 angleMajor = i * deltaAngleMajor;
        [[maybe_unused]] F32 texCoordX = i * deltaTexCoordX;

        F32 xMajor = std::cos(angleMajor);
        F32 yMajor = std::sin(angleMajor);



        for (U32 j = 0; j < numMinorSegments + 1; ++j)
        {
            F32 angleMinor = j * deltaAngleMinor;
            [[maybe_unused]] F32 texCoordY = j * deltaTexCoordY;

            F32 xMinor = std::cos(angleMinor);
            F32 yMinor = std::sin(angleMinor);

            // Store vertex data
            F32 normalX = xMajor * xMinor;
            F32 normalY = yMajor * xMinor;
            F32 normalZ = yMinor;

            vertexData.push_back(normalX * radiusMinor + xMajor * radiusMajor);
            vertexData.push_back(normalY * radiusMinor + yMajor * radiusMajor);
            vertexData.push_back(normalZ * radiusMinor);

            if constexpr (_addNormals)
            {
                vertexData.push_back(normalX);
                vertexData.push_back(normalY);
                vertexData.push_back(normalZ);
            }
            if constexpr (_addTexCoordinates)
            {
                vertexData.push_back(texCoordX);
                vertexData.push_back(texCoordY);
            }


            if (j < numMinorSegments && i < numMajorSegments)
            {
                U32 segmentIndex1 = i * (numMinorSegments + 1);
                U32 segmentIndex2 = (i + 1) * (numMinorSegments + 1);
                U32 j2 = j + 1;

                indexData.push_back(segmentIndex1 + j);
                indexData.push_back(segmentIndex1 + j2);
                indexData.push_back(segmentIndex2 + j2);
                indexData.push_back(segmentIndex2 + j2);
                indexData.push_back(segmentIndex2 + j);
                indexData.push_back(segmentIndex1 + j);
            }
        }
    }

    assert(sizeVertexData == vertexData.size());
    assert(sizeIndexData == indexData.size());

    return std::make_pair(vertexData, indexData);
}


} // namespace GDL::OpenGL::MeshGenerator
