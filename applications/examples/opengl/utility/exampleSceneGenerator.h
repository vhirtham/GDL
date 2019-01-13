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



//! @brief Stores all necessary data of a renderable object
struct Object
{
    MeshData& meshDataRef;
    Texture& textureRef;
    Mat4f modelWorld;
    std::array<F32, 2> textureScale;
    std::array<F32, 2> textureOffset;

    //! @brief Ctor
    //! @param meshData: Mesh data of the object
    //! @param texture: Texture of the object
    Object(MeshData& meshData, Texture& texture);
};



//! @brief Class to create example scenes
class ExampleSceneGenerator
{


    Program& mProgram;
    Map<String, Texture> mTextures;
    Map<String, MeshData> mMeshes;
    Vector<Object> mObjects;
    GLint mProjectionLocation;
    GLint mModelWorldLocation;
    GLint mTextureScaleLocation;
    GLint mTextureOffsetLocation;


public:
    //! @brief Ctor
    //! @param Reference to the program that should be used to render the scene
    ExampleSceneGenerator(Program& program);

    ExampleSceneGenerator(const ExampleSceneGenerator&) = delete;
    ExampleSceneGenerator(ExampleSceneGenerator&&) = delete;
    ExampleSceneGenerator& operator=(const ExampleSceneGenerator&) = delete;
    ExampleSceneGenerator& operator=(ExampleSceneGenerator&&) = delete;
    ~ExampleSceneGenerator() = default;

    //! @brief Adds a object to the scene
    //! @param meshName: Name of the mesh that should be used
    //! @param textureName: Name of the texture that should be used
    //! @param scale: Scaling factors to scale the mesh
    //! @param position: Position of the object in world coordinates
    //! @param eulerAngles: Rotation angles of the object (yaw, pitch, roll)
    //! @param texScale: Scaling factors for the texture
    //! @param texOffset: Offset of the texture
    void AddObject(const char* meshName, const char* textureName, std::array<F32, 3> scale, std::array<F32, 3> position,
                   std::array<F32, 3> eulerAngles, std::array<F32, 2> texScale = {{1, 1}}, std::array<F32, 2> texOffset = {{0, 0}});

    //! @brief Creates an example scene
    void CreateExampleScene01();

    //! @brief Renders the scene
    void Render();

    //! @brief Updates the projection matrix
    void UpdateProjection(U32 width, U32 height);

private:
    //! @brief Loads all assets and queries the necessary uniform locations
    void Initialize();

    //! @brief Loads all meshes
    void LoadMeshes();

    //! @brief Loads all textures
    void LoadTextures();

    //! @brief Queries all necessary uniform locations
    void QueryProgramUniformLocations();
};

} // namespace GDL::OpenGL


#include "applications/examples/opengl/utility/exampleSceneGenerator.inl"
