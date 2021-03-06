#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/vector.h"



namespace GDL::OpenGL::MeshGenerator
{

//! @brief Creates an array of vertex data and a corresbonding index array to render a cuboid
//! @tparam _addNormals: If true, normals are added to the vertex data
//! @tparam _addTexCoordinates: If true, texture coordinates are added to the vertex data
//! @param width: Width of the rectangle
//! @param height: Height of the rectangle
//! @param depth: Depth of the rectangle
//! @return Pair with vertex and element (index) array
template <bool _addNormals = true, bool _addTexCoordinates = false>
std::pair<Vector<F32>, Vector<U32>> CreateCuboid(F32 width = 1, F32 height = 1, F32 depth = 1);

//! @brief Creates an array of vertex data and a corresbonding index array to render a rectangle
//! @tparam _addNormals: If true, normals are added to the vertex data
//! @tparam _addTexCoordinates: If true, texture coordinates are added to the vertex data
//! @param width: Width of the rectangle
//! @param height: Height of the rectangle
//! @return Pair with vertex and element (index) array
template <bool _addNormals = false, bool _addTexCoordinates = false>
std::pair<Vector<F32>, Vector<U32>> CreateRectangle(F32 width = 1, F32 height = 1);

//! @brief Creates an array of vertex data and a corresbonding index array to render a sphere
//! @tparam _addNormals: If true, normals are added to the vertex data
//! @tparam _addTexCoordinates: If true, texture coordinates are added to the vertex data
//! @param numHorizontalSegments: Number of segments in horizontal direction
//! @param numVerticalSegments: Number of segments in vertical direction
//! @param radius: Radius of the sphere
//! @remark The created sphere creates texturing artifacts at the poles
template <bool _addNormals = false, bool _addTexCoordinates = false>
std::pair<Vector<F32>, Vector<U32>> CreateSphere(U32 numHorizontalSegments, U32 numVerticalSegments, F32 radius = 1);

//! @brief Creates an array of vertex data and a corresbonding index array to render a torus
//! @tparam _addNormals: If true, normals are added to the vertex data
//! @tparam _addTexCoordinates: If true, texture coordinates are added to the vertex data
//! @param radiusMajor: Major radius of the torus
//! @param radiusMinor: Minor radius of the torus
//! @param numMajorSegments: The number of segments along the major circle
//! @param numMinorSegments: The number of segments along the minor circle
//! @return Pair with vertex and element (index) array
template <bool _addNormals = true, bool _addTexCoordinates = false>
std::pair<Vector<F32>, Vector<U32>> CreateTorus(F32 radiusMajor, F32 radiusMinor, U32 numMajorSegments,
                                                U32 numMinorSegments);
} // namespace GDL::OpenGL::MeshGenerator


#include "applications/examples/opengl/utility/meshGenerator.inl"
