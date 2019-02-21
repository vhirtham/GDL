#pragma once

#include "gdl/base/string.h"
#include "gdl/base/container/map.h"
#include "gdl/base/container/vector.h"
#include "gdl/math/transformations4.h"
#include "gdl/math/vec3.h"
#include "gdl/math/vec4.h"
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

//! @brief Stores data of a mesh
struct MeshData
{
    VertexArrayObject VAO;
    BufferObject VBO;
    BufferObject EBO;
    I32 numIndices;

    //! @brief Ctor
    //! @param vertexData: Vertex data of the mesh
    //! @param elementData: Element data of the mesh
    MeshData(const Vector<F32>& vertexData, const Vector<U32>& elementData);
};



//! @brief Stores material data
struct Material
{
    Texture& albedo;
    Texture& specular;
    F32 reflectivity;

    //! @brief Ctor
    //! @param albedoTexture: Material albedo (color) texture
    //! @param specularTexture: Specular texture
    //! @param reflectivityValue: Reflectivity value
    Material(Texture& albedoTexture, Texture& specularTerm, F32 reflectivityValue)
        : albedo{albedoTexture}
        , specular{specularTerm}
        , reflectivity{reflectivityValue}
    {
    }
};



//! @brief Stores all necessary data of a renderable object
struct Object
{
    MeshData& meshDataRef;
    Material& materialRef;
    Mat4f modelWorld;
    std::array<F32, 2> textureScale;
    std::array<F32, 2> textureOffset;

    //! @brief Ctor
    //! @param meshData: Mesh data of the object
    //! @param texture: Texture of the object
    Object(MeshData& meshData, Material& texture);
};



//! @brief Class to create example scenes
class ExampleSceneGenerator
{


    Program& mProgram;
    Map<String, Texture> mTextures;
    Map<String, Material> mMaterials;
    Map<String, MeshData> mMeshes;
    Vector<Object> mObjects;

    U32 mAlbedoTextureUnit = 0;
    U32 mSpecularTextureUnit = 1;

    GLint mProjectionLocation;
    GLint mModelWorldLocation;
    GLint mTextureScaleLocation;
    GLint mTextureOffsetLocation;
    GLint mMaterialReflectivityLocation;


public:
    //! @brief Ctor
    //! @param Reference to the program that should be used to render the scene
    ExampleSceneGenerator(Program& program);

    ExampleSceneGenerator(const ExampleSceneGenerator&) = delete;
    ExampleSceneGenerator(ExampleSceneGenerator&&) = delete;
    ExampleSceneGenerator& operator=(const ExampleSceneGenerator&) = delete;
    ExampleSceneGenerator& operator=(ExampleSceneGenerator&&) = delete;
    ~ExampleSceneGenerator() = default;

    //! @brief Adds a material
    //! @param materialName: Name of the material
    //! @param albedo: Albedo texture (material color)
    //! @param specular: Specular map texture;
    //! @param reflectivity: Material reflectivity
    void AddMaterial(const char* materialName, Texture& albedo, Texture& specular, F32 reflectivity);

    //! @brief Adds a object to the scene
    //! @param meshName: Name of the mesh that should be used
    //! @param textureName: Name of the texture that should be used
    //! @param scale: Scaling factors to scale the mesh
    //! @param position: Position of the object in world coordinates
    //! @param eulerAngles: Rotation angles of the object (yaw, pitch, roll)
    //! @param texScale: Scaling factors for the texture
    //! @param texOffset: Offset of the texture
    void AddObject(const char* meshName, const char* textureName, std::array<F32, 3> scale, std::array<F32, 3> position,
                   std::array<F32, 3> eulerAngles, std::array<F32, 2> texScale = {{1, 1}},
                   std::array<F32, 2> texOffset = {{0, 0}});

    //! @brief Creates an example scene
    void CreateExampleScene01();

    //! @brief Returns a requested texture
    //! @param textureName: Name of the texture
    //! @return Requested texture
    Texture& GetTexture(const char* textureName);

    //! @brief Renders the scene
    void Render();

    //! @brief Updates the projection matrix
    //! @param projectionMatrix: Projection matrix
    void UpdateProjection(Mat4f projectionMatrix);

private:
    //! @brief Loads all assets and queries the necessary uniform locations
    void Initialize();

    //! @brief Loads all meshes
    void LoadMeshes();

    //! @brief Loads all textures
    void LoadTextures();


    //! @brief Creates all available amterials
    void CreateMaterials();

    //! @brief Queries all necessary uniform locations
    void QueryProgramUniformLocations();
};

} // namespace GDL::OpenGL


#include "applications/examples/opengl/utility/exampleSceneGenerator.inl"
