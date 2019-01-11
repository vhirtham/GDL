#pragma once

#include "gdl/base/string.h"
#include "gdl/base/container/map.h"
#include "gdl/base/container/vector.h"
#include "gdl/math/transformationMatrix.h"
#include "gdl/rendering/textures/textureData2d.h"
#include "gdl/rendering/textures/textureFile.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/texture.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

#include "applications/examples/opengl/utility/meshGenerator.h"

#include <array>



namespace GDL::OpenGL
{
class DefaultScene01
{
    struct MeshData
    {
        VertexArrayObject VAO;
        BufferObject VBO;
        BufferObject EBO;
        I32 numIndices;

        MeshData(const Vector<F32>& vertexData, const Vector<U32>& elementData)
            : VBO(vertexData, GL_STATIC_DRAW)
            , EBO(elementData, GL_STATIC_DRAW)
            , numIndices{static_cast<I32>(elementData.size())}
        {
            VAO.EnableAttribute(0, 0, VBO, GL_FLOAT, 3, sizeof(F32) * 8);
            VAO.EnableAttribute(1, 0, GL_FLOAT, 3, sizeof(F32) * 3);
            VAO.EnableAttribute(2, 0, GL_FLOAT, 2, sizeof(F32) * 6);
            VAO.SetElementBuffer(EBO);
        }
    };

    struct Object
    {
        MeshData& meshDataRef;
        Texture& textureRef;
        Mat4f modelWorld;
        std::array<F32, 2> textureScale;

        Object(MeshData& meshData, Texture& texture)
            : meshDataRef{meshData}
            , textureRef{texture}
        {
        }
    };

    Program& mProgram;
    Map<String, Texture> mTextures;
    Map<String, MeshData> mMeshes;
    Vector<Object> mObjects;
    GLint mProjectionLocation;
    GLint mModelWorldLocation;
    GLint mTextureScaleXLocation;
    GLint mTextureScaleYLocation;


public:
    DefaultScene01(Program& program)
        : mProgram{program}
    {
        Initialize();
    }

    DefaultScene01(const DefaultScene01&) = delete;
    DefaultScene01(DefaultScene01&&) = delete;
    DefaultScene01& operator=(const DefaultScene01&) = delete;
    DefaultScene01& operator=(DefaultScene01&&) = delete;
    ~DefaultScene01() = default;



    void Render()
    {
        for (auto& object : mObjects)
        {
            object.meshDataRef.VAO.Bind();
            object.textureRef.Bind(0);
            mProgram.SetUniform(mModelWorldLocation, object.modelWorld);
            mProgram.SetUniform(mTextureScaleXLocation, object.textureScale[0]);
            mProgram.SetUniform(mTextureScaleYLocation, object.textureScale[1]);
            glDrawElements(GL_TRIANGLES, object.meshDataRef.numIndices, GL_UNSIGNED_INT, nullptr);
        }
    }

    void AddObject(const char* meshName, const char* textureName, std::array<F32, 3> scale, std::array<F32, 3> position,
                   std::array<F32, 3> eulerAngles, std::array<F32, 2> texScale = {{1, 1}})
    {
        auto meshIterator = mMeshes.find(meshName);
        auto textureIterator = mTextures.find(textureName);

        EXCEPTION(meshIterator == mMeshes.end(), "Could not find mesh");
        EXCEPTION(textureIterator == mTextures.end(), "Could not find texture");

        mObjects.emplace_back(meshIterator->second, textureIterator->second);
        mObjects.back().modelWorld = TransformationMatrix4::Translation(position[0], position[1], position[2]) *
                                     TransformationMatrix4::RotationZ(eulerAngles[2]) *
                                     TransformationMatrix4::RotationX(eulerAngles[1]) *
                                     TransformationMatrix4::RotationY(eulerAngles[0]) *
                                     TransformationMatrix4::Scale(scale[0], scale[1], scale[2]);
        mObjects.back().textureScale = texScale;
    }

    void UpdateProjection(U32 width, U32 height)
    {
        mProgram.SetUniform(mProjectionLocation,
                            TransformationMatrix4::PerspectiveProjection(45.f, static_cast<F32>(width),
                                                                         static_cast<F32>(height), 0.1f, 100.f));
    }

    void CreateDefaultScene01()
    {
        AddObject("rectangle", "gravel", {{20, 20, 1}}, {{0, -1, -10}}, {{0, static_cast<F32>(-M_PI) / 2.f, 0}},
                  {{5, 5}});
        AddObject("torus", "redBrick02", {{1, 1, 1}}, {{0, 0, -10}}, {{static_cast<F32>(-M_PI) / 4.f, 0, 0}}, {{2, 1}});
    }

private:
    void Initialize()
    {
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

        for(auto& texture :mTextures)
        {
            texture.second.CreateMipMaps();
            texture.second.SetMinifyingFilter(GL_LINEAR_MIPMAP_LINEAR);
            texture.second.SetMagnifyingFilter(GL_LINEAR);
        }


        auto [vertexDataRectagle, elementDataRectangle] = MeshGenerator::CreateRectangle<true, true>();
        mMeshes.emplace(std::piecewise_construct, std::forward_as_tuple("rectangle"),
                        std::forward_as_tuple(vertexDataRectagle, elementDataRectangle));

        auto [vertexDataTorus, elementDataTorus] = MeshGenerator::CreateTorus<true, true>(0.7f, 0.3f, 32, 32);
        mMeshes.emplace(std::piecewise_construct, std::forward_as_tuple("torus"),
                        std::forward_as_tuple(vertexDataTorus, elementDataTorus));



        mProjectionLocation = mProgram.QueryUniformLocation("projectionMatrix");
        mModelWorldLocation = mProgram.QueryUniformLocation("modelWorldMatrix");
        mTextureScaleXLocation = mProgram.QueryUniformLocation("textureScaleX");
        mTextureScaleYLocation = mProgram.QueryUniformLocation("textureScaleY");
    }
};

} // namespace GDL::OpenGL
