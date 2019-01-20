#include "applications/tools/models/modelImporter/src/model.h"


#include "gdl/base/exception.h"
#include "gdl/base/string.h"


#include <assimp/postprocess.h>

using namespace GDL;

namespace MI
{


void Model::Draw()
{
    for (auto& mesh : mMeshes)
        mesh.Draw();
}

Model::Model(const char* fileName)
{
    Initialize(fileName);
}

void Model::Initialize(const char* fileName)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

    EXCEPTION(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode,
              MakeString("Could not load file with assimp:\n", importer.GetErrorString()).c_str());

    this->ProcessNode(*(scene->mRootNode), *scene);
}



void Model::ProcessNode(aiNode& node, const aiScene& scene)
{
    for (U32 i = 0; i < node.mNumMeshes; ++i)
    {
        aiMesh& mesh = *scene.mMeshes[node.mMeshes[i]];
        mMeshes.emplace_back(ProcessMesh(mesh, scene));
    }

    for (U32 i = 0; i < node.mNumChildren; ++i)
        ProcessNode(*node.mChildren[i], scene);
}



Mesh Model::ProcessMesh(aiMesh& mesh, const aiScene& scene)
{

    Vector<F32> vertices;
    Vector<U32> indices;


    for (U32 i = 0; i < mesh.mNumVertices; ++i)
    {
        vertices.push_back(mesh.mVertices[i].x);
        vertices.push_back(mesh.mVertices[i].y);
        vertices.push_back(mesh.mVertices[i].z);
        //        vertex.normal[0] = mesh->mNormals[i].x;
        //        vertex.normal[1] = mesh->mNormals[i].y;
        //        vertex.normal[2] = mesh->mNormals[i].z;
        //        if (mesh->mTextureCoords[0])
        //        {
        //            vertex.texCoords[0] = mesh->mTextureCoords[0][i].x;
        //            vertex.texCoords[1] = mesh->mTextureCoords[0][i].y;
        //        }
        //        else
        //        {
        //            vertex.texCoords[0] = 0;
        //            vertex.texCoords[1] = 0;
        //        }
        //        vertices.push_back(vertex);
    }
    // process indices
    for (U32 i = 0; i < mesh.mNumFaces; ++i)
    {
        aiFace& face = mesh.mFaces[i];
        for (U32 j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    //    if (mesh->mMaterialIndex >= 0)
    //    {
    //        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    //        Vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    //        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    //        Vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    //        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //    }

    return Mesh(vertices, indices);
}


} // namespace MI
