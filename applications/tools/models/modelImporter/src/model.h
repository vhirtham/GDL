#pragma once

#include "applications/tools/models/modelImporter/src/mesh.h"

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/string.h"
#include "gdl/base/container/map.h"
#include "gdl/base/container/vector.h"
#include "gdl/rendering/openGL/core/texture.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>



namespace MI
{
using namespace GDL;



class Model
{

    String mModelDirectory;
    Vector<Mesh> mMeshes;
    Map<String, Texture> mDiffuseTextures;

public:
    void Draw();

    Model(const char* fileName);

private:
    void Initialize(const char* fileName);

    void LoadMaterials(const aiScene& scene);

    void ProcessNode(aiNode& node, const aiScene& scene);

    Mesh ProcessMesh(aiMesh& mesh, const aiScene& scene);
};

} // namespace MI
