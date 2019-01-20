#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/vector.h"

#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

namespace MI
{
using namespace GDL;
using namespace GDL::OpenGL;

class Mesh
{

    Vector<F32> mVertices;
    Vector<U32> mIndices;
    VertexArrayObject mVAO;
    BufferObject mVBO;
    BufferObject mEBO;

public:

    Mesh(Vector<F32> vertices, Vector<U32> indices);

    void Draw();
};
} // namespace MI
