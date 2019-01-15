#pragma once

#include "applications/examples/opengl/utility/exampleSceneGenerator.h"

#include "gdl/rendering/textures/textureData2d.h"

namespace GDL::OpenGL
{


MeshData::MeshData(const Vector<F32>& vertexData, const Vector<U32>& elementData)
    : VBO(vertexData, GL_STATIC_DRAW)
    , EBO(elementData, GL_STATIC_DRAW)
    , numIndices{static_cast<I32>(elementData.size())}
{
    VAO.EnableAttribute(0, 0, VBO, GL_FLOAT, 3, sizeof(F32) * 8);
    VAO.EnableAttribute(1, 0, GL_FLOAT, 3, sizeof(F32) * 3);
    VAO.EnableAttribute(2, 0, GL_FLOAT, 2, sizeof(F32) * 6);
    VAO.SetElementBuffer(EBO);
}



Object::Object(MeshData& meshData, Material& texture)
    : meshDataRef{meshData}
    , materialRef{texture}
{
}



ExampleSceneGenerator::ExampleSceneGenerator(Program& program)
    : mProgram{program}
{
    Initialize();
}



void ExampleSceneGenerator::AddMaterial(const char* materialName, Texture& albedo, Texture& specular, F32 reflectivity)
{
    mMaterials.emplace(std::piecewise_construct, std::forward_as_tuple(materialName),
                       std::forward_as_tuple(albedo, specular, reflectivity));
}



void ExampleSceneGenerator::AddObject(const char* meshName, const char* textureName, std::array<F32, 3> scale,
                                      std::array<F32, 3> position, std::array<F32, 3> eulerAngles,
                                      std::array<F32, 2> texScale, std::array<F32, 2> texOffset)
{
    auto meshIterator = mMeshes.find(meshName);
    auto materialIterator = mMaterials.find(textureName);

    EXCEPTION(meshIterator == mMeshes.end(), "Could not find mesh");
    EXCEPTION(materialIterator == mMaterials.end(), "Could not find material");

    mObjects.emplace_back(meshIterator->second, materialIterator->second);
    mObjects.back().modelWorld = TransformationMatrix4::Translation(position[0], position[1], position[2]) *
                                 TransformationMatrix4::RotationZ(-eulerAngles[0]) *
                                 TransformationMatrix4::RotationX(-eulerAngles[1]) *
                                 TransformationMatrix4::RotationY(-eulerAngles[2]) *
                                 TransformationMatrix4::Scale(scale[0], scale[1], scale[2]);
    mObjects.back().textureScale = texScale;
    mObjects.back().textureOffset = texOffset;
}



void ExampleSceneGenerator::CreateExampleScene01()
{
    constexpr F32 pi = static_cast<F32>(M_PI);

    AddObject("rectangle", "gravel", {{20, 20, 1}}, {{0, 0, 0}}, {{0, 0, 0}}, {{5, 5}}, {{0, 0}});
    AddObject("rectangle", "redBrick01", {{20, 2, 1}}, {{0, 10, 1}}, {{0, -pi / 2.f, 0}}, {{10, 1}}, {{0, 0}});
    AddObject("torus", "redBrick02", {{1, 1, 1}}, {{0, 5, 1}}, {{0, -pi / 2.f, pi / 4.f}}, {{2, 1}});
    AddObject("sphere", "brickPavement04", {{1, 1, 1}}, {{-3, 3, 1}}, {{0, 0, 0}}, {{2, 1}});

    AddObject("cuboid", "brickPavement02", {{3, 3, 5}}, {{3, 8, 2.5}}, {{0, 0, 0}}, {{1.5, 2.5}}, {{0, 0}});
    AddObject("cuboid", "brickPavement01", {{1, 1, 1}}, {{2, 6., 0}}, {{0, 0, 0}}, {{1, 1}}, {{0, 0}});
}

Texture& ExampleSceneGenerator::GetTexture(const char* textureName)
{
    auto textureIterator = mTextures.find(textureName);
    EXCEPTION(textureIterator == mTextures.end(), MakeString("Could not find texture \"", textureName, "\"").c_str());
    return textureIterator->second;
}



void ExampleSceneGenerator::Render()
{
    mProgram.Use();

    for (auto& object : mObjects)
    {

        object.meshDataRef.VAO.Bind();
        object.materialRef.albedo.Bind(mAlbedoTextureUnit);
        object.materialRef.specular.Bind(mSpecularTextureUnit);

        // Set uniforms
        mProgram.SetUniform(mModelWorldLocation, object.modelWorld);

        if (mMaterialReflectivityLocation>-1)
            mProgram.SetUniform(mMaterialReflectivityLocation, object.materialRef.reflectivity);
        if (mTextureScaleLocation > -1)
            mProgram.SetUniform(mTextureScaleLocation, object.textureScale);
        if (mTextureOffsetLocation > -1)
            mProgram.SetUniform(mTextureOffsetLocation, object.textureOffset);

        glDrawElements(GL_TRIANGLES, object.meshDataRef.numIndices, GL_UNSIGNED_INT, nullptr);
    }
}



void ExampleSceneGenerator::UpdateProjection(Mat4f projectionMatrix)
{
    mProgram.SetUniform(mProjectionLocation, projectionMatrix);
}



void ExampleSceneGenerator::Initialize()
{
    LoadTextures();
    CreateMaterials();
    LoadMeshes();
    QueryProgramUniformLocations();
}



void ExampleSceneGenerator::LoadMeshes()
{
    auto [vertexDataCuboid, elementDataCuboid] = MeshGenerator::CreateCuboid<true, true>();
    mMeshes.emplace(std::piecewise_construct, std::forward_as_tuple("cuboid"),
                    std::forward_as_tuple(vertexDataCuboid, elementDataCuboid));

    auto [vertexDataSphere, elementDataSphere] = MeshGenerator::CreateSphere<true, true>(32, 32, 1);
    mMeshes.emplace(std::piecewise_construct, std::forward_as_tuple("sphere"),
                    std::forward_as_tuple(vertexDataSphere, elementDataSphere));

    auto [vertexDataRectagle, elementDataRectangle] = MeshGenerator::CreateRectangle<true, true>();
    mMeshes.emplace(std::piecewise_construct, std::forward_as_tuple("rectangle"),
                    std::forward_as_tuple(vertexDataRectagle, elementDataRectangle));


    auto [vertexDataTorus, elementDataTorus] = MeshGenerator::CreateTorus<true, true>(0.7f, 0.3f, 32, 32);
    mMeshes.emplace(std::piecewise_construct, std::forward_as_tuple("torus"),
                    std::forward_as_tuple(vertexDataTorus, elementDataTorus));
}



void ExampleSceneGenerator::LoadTextures()
{

    mTextures.emplace("white", TextureData2d(1, 1, 4, {{255, 255, 255, 255}}));
    mTextures.emplace("test", TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/testImage.tex").c_str()));
    mTextures.emplace("redBrick01", TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/redBrick01_2k.tex").c_str()));
    mTextures.emplace("redBrick02", TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/redBrick02_2k.tex").c_str()));
    mTextures.emplace("brickPavement01",
                      TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/brickPavement01_2k.tex").c_str()));
    mTextures.emplace("brickPavement02",
                      TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/brickPavement02_2k.tex").c_str()));
    mTextures.emplace("brickPavement03",
                      TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/brickPavement03_2k.tex").c_str()));
    mTextures.emplace("brickPavement04",
                      TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/brickPavement04_2k.tex").c_str()));
    mTextures.emplace("gravel", TextureFile::Read(MakeString(TEXTURE_DIRECTORY, "/gravelWithDirt_2k.tex").c_str()));

    for (auto& texture : mTextures)
    {
        texture.second.CreateMipMaps();
        texture.second.SetMinifyingFilter(GL_LINEAR_MIPMAP_LINEAR);
        texture.second.SetMagnifyingFilter(GL_LINEAR);
    }
}



void ExampleSceneGenerator::CreateMaterials()
{
    AddMaterial("gravel", GetTexture("gravel"), GetTexture("gravel"), 1);
    AddMaterial("redBrick01", GetTexture("redBrick01"), GetTexture("redBrick01"), 32);
    AddMaterial("redBrick02", GetTexture("redBrick02"), GetTexture("white"), 32);
    AddMaterial("brickPavement01", GetTexture("brickPavement01"), GetTexture("brickPavement01"), 32);
    AddMaterial("brickPavement02", GetTexture("brickPavement02"), GetTexture("brickPavement02"), 32);
    AddMaterial("brickPavement03", GetTexture("brickPavement03"), GetTexture("brickPavement03"), 32);
    AddMaterial("brickPavement04", GetTexture("brickPavement04"), GetTexture("brickPavement04"), 32);
}



void ExampleSceneGenerator::QueryProgramUniformLocations()
{
    mProjectionLocation = mProgram.QueryUniformLocation("projection");
    mModelWorldLocation = mProgram.QueryUniformLocation("modelWorld");

    // Optional uniforms
    if (mProgram.QueryHasUniform("textureScale"))
        mTextureScaleLocation = mProgram.QueryUniformLocation("textureScale");
    else
        mTextureScaleLocation = -1;

    if (mProgram.QueryHasUniform("textureOffset"))
        mTextureOffsetLocation = mProgram.QueryUniformLocation("textureOffset");
    else
        mTextureOffsetLocation = -1;

    if (mProgram.QueryHasUniform("material.reflectivity"))
        mMaterialReflectivityLocation = mProgram.QueryUniformLocation("material.reflectivity");
    else
        mMaterialReflectivityLocation = -1;
}

} // namespace GDL::OpenGL
