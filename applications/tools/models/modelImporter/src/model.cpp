#include "applications/tools/models/modelImporter/src/model.h"


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/exception.h"
#include "gdl/base/string.h"
#include "gdl/rendering/textures/textureData2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

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


    String filePathString(fileName);
    mModelDirectory = filePathString.substr(0, filePathString.find_last_of('/'));
    LoadMaterials(*scene);
    ProcessNode(*(scene->mRootNode), *scene);
}



void Model::LoadMaterials(const aiScene& scene)
{
    for (U32 i = 0; i < scene.mNumMaterials; ++i)
    {
        aiMaterial& material = *scene.mMaterials[i];
        U32 numDiffuseTextures = material.GetTextureCount(aiTextureType_DIFFUSE);
        for (U32 j = 0; j < numDiffuseTextures; ++j)
        {
            aiString fileName;
            material.GetTexture(aiTextureType_DIFFUSE, j, &fileName);

            String filePath = MakeString(mModelDirectory, "/", fileName.C_Str());
            I32 width, height, numChannels;
            U8* pixelData = stbi_load(filePath.c_str(), &width, &height, &numChannels, STBI_default);
            TextureData2d textureData(static_cast<U32>(width), static_cast<U32>(height), static_cast<U32>(numChannels),
                                      pixelData);
            mDiffuseTextures.emplace(fileName.C_Str(), textureData);
        }
    }
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

    U32 numTexCoords = 0;
    for (U32 i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
        if (mesh.mTextureCoords[i] != nullptr)
            ++numTexCoords;

    EXCEPTION(numTexCoords > 1, "Model uses more than 1 texture coordinate. Only 1 is supported at the moment");

    for (U32 i = 0; i < mesh.mNumVertices; ++i)
    {
        vertices.push_back(mesh.mVertices[i].x);
        vertices.push_back(mesh.mVertices[i].y);
        vertices.push_back(mesh.mVertices[i].z);
        vertices.push_back(mesh.mNormals[i].x);
        vertices.push_back(mesh.mNormals[i].y);
        vertices.push_back(mesh.mNormals[i].z);
        for (U32 j = 0; j < numTexCoords; ++j)
        {
            vertices.push_back(mesh.mTextureCoords[j][i].x);
            vertices.push_back(mesh.mTextureCoords[j][i].y);
        }
    }

    // process indices
    for (U32 i = 0; i < mesh.mNumFaces; ++i)
    {
        aiFace& face = mesh.mFaces[i];
        for (U32 j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    aiMaterial& material = *scene.mMaterials[mesh.mMaterialIndex];
    aiString textureName;

    Vector<std::pair<U32, std::reference_wrapper<Texture>>> textures;

    U32 numDiffuseTextures = material.GetTextureCount(aiTextureType_DIFFUSE);
    for (U32 i = 0; i < numDiffuseTextures; ++i)
    {
        material.GetTexture(aiTextureType_DIFFUSE, i, &textureName);
        auto diffuseTexturesIterator = mDiffuseTextures.find(textureName.C_Str());
        EXCEPTION(diffuseTexturesIterator == mDiffuseTextures.end(), "Texture not found.");
        textures.emplace_back(0, diffuseTexturesIterator->second);
    }

    return Mesh(vertices, indices, numTexCoords, textures);
}


} // namespace MI
