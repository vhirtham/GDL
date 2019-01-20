#pragma once

#include "applications/tools/models/modelImporter/src/mesh.h"

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/vector.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>



namespace MI
{


class Model
{

    GDL::Vector<Mesh> mMeshes;


public:
    void Draw();

    Model(const char* fileName);

private:
    void Initialize(const char* fileName);

    void ProcessNode(aiNode& node, const aiScene& scene);

    Mesh ProcessMesh(aiMesh& mesh, const aiScene& scene);

};

} // namespace MI
