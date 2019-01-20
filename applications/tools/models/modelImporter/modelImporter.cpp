#include "src/openGLWidget.h"
#include "src/mainWindow.h"
#include "src/model.h"

#include "gdl/base/container/vector.h"
#include "gdl/base/string.h"
#include "gdl/math/vec3.h"
#include "gdl/math/transformationMatrix.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



#include <QApplication>
#include <QOpenGLWidget>

#include <sstream>
#include <iostream>

using namespace GDL;

struct Vertex
{
    std::array<F32, 3> position;
    std::array<F32, 3> normal;
    std::array<F32, 2> texCoords;
};


struct Texture
{
    GLuint id;
    String type;
};


class Mesh2
{
public:
    Vector<Vertex> vertices;
    Vector<GLuint> indices;
    Vector<Texture> textures;

    Mesh2(Vector<Vertex> verticesP, Vector<GLuint> indicesP, Vector<Texture> texturesP)
        : vertices{verticesP}
        , indices{indicesP}
        , textures{texturesP}
    {
        SetupMesh();
    }

    void Draw(OpenGL::Program& program)
    {
        GLuint diffuseNr = 1;
        GLuint specularNR = 1;
        for (GLuint i = 0; i < textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::stringstream ss;
            String number;
            String name = textures[i].type;
            if (name == "texture_diffuse")
                ;
            ss << diffuseNr++;
            number = ss.str();

            glUniform1f(glGetUniformLocation(program.GetHandle(), MakeString("material.", name, number).c_str()), i);
            glBindTexture(GL_PROXY_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    GLuint VAO, VBO, EBO;

    void SetupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }
};



class Model2
{
    Vector<Mesh2> meshes;
    String directory;

public:
    Model2(const char* path)
    {
        this->LoadModel(path);
    }

    void LoadModel(String path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        this->directory = path.substr(0, path.find_last_of('/'));
        this->ProcessNode(scene->mRootNode, scene);
    }

    void ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (GLuint i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }
        for (GLuint i = 0; i < node->mNumChildren; ++i)
            ProcessNode(node->mChildren[i], scene);
    }

    Mesh2 ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        Vector<Vertex> vertices;
        Vector<GLuint> indices;
        Vector<Texture> textures;

        for (GLuint i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex;
            // process vertices
            vertex.position[0] = mesh->mVertices[i].x;
            vertex.position[1] = mesh->mVertices[i].y;
            vertex.position[2] = mesh->mVertices[i].z;
            vertex.normal[0] = mesh->mNormals[i].x;
            vertex.normal[1] = mesh->mNormals[i].y;
            vertex.normal[2] = mesh->mNormals[i].z;
            if (mesh->mTextureCoords[0])
            {
                vertex.texCoords[0] = mesh->mTextureCoords[0][i].x;
                vertex.texCoords[1] = mesh->mTextureCoords[0][i].y;
            }
            else
            {
                vertex.texCoords[0] = 0;
                vertex.texCoords[1] = 0;
            }
            vertices.push_back(vertex);
        }
        // process indices
        for (GLuint i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for (GLuint j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }

        // process materials
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            Vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            Vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh2(vertices, indices, textures);
    }

    Vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType textureType, String typeName)
    {
        Vector<Texture> textures;
        for (GLuint i = 0; i < material->GetTextureCount(textureType); ++i)
        {
            aiString str;
            material->GetTexture(textureType, i, &str);
            Texture texture;
            texture.id = 0;
            texture.type = typeName;
            textures.push_back(texture);
        }
        return textures;
    }

private:
};

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1600, 900);
    OpenGLWidget ogl(&w);
    ogl.resize(1000, 800);
    w.show();


    // Shader Code ##############################

    const char vertexShaderCode[] = R"glsl(
            #version 430

            layout (location=0) in vec3 VertexPosition;

            uniform mat4 projection;
            uniform mat4 modelWorld;

            void main(void)
            {

                gl_Position = projection * modelWorld * vec4(VertexPosition,1.0);
            }
            )glsl";

    const char fragmentShaderCode[] = R"glsl(
            #version 430

            float LinearDepth(float depth)
            {
                float near = 0.1;
                float far = 100;
                float z = 2 * depth - 1;
                return (2.f * near) / (far + near -z * (far - near));
            }

            out vec4 FragColor;
            void main(void)
            {
                FragColor = vec4(vec3(LinearDepth(gl_FragCoord.z)),1);
            }
            )glsl";

    using namespace GDL;
    using namespace GDL::OpenGL;
    // Create Shader and Program ################

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    Program program(vertexShader, fragmentShader);



    // Assimp

    //    Model2 model(modelFileName.c_str());
    //    Assimp::Importer importer;
    //    const aiScene* scene = importer.ReadFile(modelFileName.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    //    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    //        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;

    program.Use();
    program.SetUniform(program.QueryUniformLocation("projection"),
                       TransformationMatrix4::PerspectiveProjection(45, 1000, 800, 0.1f, 100));
    program.SetUniform(program.QueryUniformLocation("modelWorld"),
                       TransformationMatrix4::Translation(0, 0, -50) *
                               TransformationMatrix4::Scale(0.05f, 0.05f, 0.05f));

    ogl.update();

    EXCEPTION(glGetError() != GL_NO_ERROR, "OpenGl Error");
    return a.exec();
}
