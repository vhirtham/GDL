#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/map.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/texture.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

namespace MI
{
using namespace GDL;
using namespace GDL::OpenGL;

class Mesh
{

    Vector<F32> mVertices;
    Vector<U32> mIndices;
    Vector<std::pair<U32, std::reference_wrapper<Texture>>> mTextures;
    VertexArrayObject mVAO;
    BufferObject mVBO;
    BufferObject mEBO;
    U32 mNumTextureCoordinates;

public:
    Mesh(Vector<F32> vertices, Vector<U32> indices, U32 numTextureCoordinates,
         Vector<std::pair<U32, std::reference_wrapper<Texture>>> textures);

    void Draw();
};
} // namespace MI
