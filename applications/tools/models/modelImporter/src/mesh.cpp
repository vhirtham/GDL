#include "applications/tools/models/modelImporter/src/mesh.h"



namespace MI
{

Mesh::Mesh(Vector<F32> vertices, Vector<U32> indices, U32 numTextureCoordinates,
           Vector<std::pair<U32, std::reference_wrapper<Texture>>> textures)
    : mVertices{vertices}
    , mIndices{indices}
    , mTextures{textures}
    , mVBO(vertices, GL_STATIC_DRAW)
    , mEBO(indices, GL_STATIC_DRAW)
    , mNumTextureCoordinates{numTextureCoordinates}
{
    constexpr U32 numPositionValues = 3;
    constexpr U32 numNormalValues = 3;
    const U32 numTexCoordValues = 2 * numTextureCoordinates;
    const U32 numTotalValues = numPositionValues + numNormalValues + numTexCoordValues;

    const GLsizei stride = static_cast<GLsizei>(numTotalValues * sizeof(F32));

    mVAO.EnableAttribute(0, 0, mVBO, GL_FLOAT, 3, stride);
    mVAO.EnableAttribute(1, 0, GL_FLOAT, 3, 12);
    mVAO.EnableAttribute(2, 0, GL_FLOAT, 2, 24);
    mVAO.SetElementBuffer(mEBO);
}

void Mesh::Draw()
{
    mVAO.Bind();
    for (auto& textureData : mTextures)
        textureData.second.get().Bind(textureData.first);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
}

} // namespace MI
