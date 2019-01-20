#include "applications/tools/models/modelImporter/src/mesh.h"



namespace MI
{

Mesh::Mesh(Vector<F32> vertices, Vector<U32> indices)
    : mVertices{vertices}
    , mIndices{indices}
    , mVBO(vertices, GL_STATIC_DRAW)
    , mEBO(indices, GL_STATIC_DRAW)
{
    mVAO.EnableAttribute(0, 0, mVBO, GL_FLOAT, 3, 12);
    mVAO.SetElementBuffer(mEBO);
}

void Mesh::Draw()
{
    mVAO.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
}

} // namespace MI
