#include "gkpch.h"
#include "Model.h"
#include <assimp/pbrmaterial.h>

namespace Gecko
{
    void Model::LoadFromFile(std::string path)
    {
        meshes.clear();
        textures.clear();

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            GK_LOG(BOLDRED "[ASSIMP ERROR]" RESET) << importer.GetErrorString() << std::endl;
            return;
        }

        this->directory = path.substr(0, path.find_last_of('/'));

        this->ProcessNode(scene->mRootNode, scene);
    }
    void Model::ProcessNode(aiNode *node, const aiScene *scene)
    {
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

            this->meshes.push_back(this->ProcessMesh(mesh, scene));
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            this->ProcessNode(node->mChildren[i], scene);
        }
    }
    Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Texture> textures;

        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;

                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;

                vertex.texCoord = vec;

                //Segmentation  fault
                
                 vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector; 
            }
            else
            {
                vertex.texCoord = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }
        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (uint32_t j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<Texture> albedoMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "albedoTexture");
            textures.insert(textures.end(), albedoMaps.begin(), albedoMaps.end());

            std::vector<Texture> specularMaps = this->LoadMaterialTextures(material, aiTextureType_SPECULAR, "specularTexture");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            std::vector<Texture> metallicMaps = this->LoadMaterialTextures(material, aiTextureType_SPECULAR, "metallicTexture");
            textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());

            std::vector<Texture> roughnessMaps = this->LoadMaterialTextures(material, aiTextureType_SHININESS, "roughnessTexture");
            textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

            std::vector<Texture> normalMaps = this->LoadMaterialTextures(material, aiTextureType_HEIGHT, "normalTexture");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

            std::vector<Texture> AOMaps = this->LoadMaterialTextures(material, aiTextureType_AMBIENT, "AOTexture");
            textures.insert(textures.end(), AOMaps.begin(), AOMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }
    std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;

        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;

            for (uint32_t j = 0; j < this->textures.size(); j++)
            {
                if (this->textures[j].GetPath() == str.C_Str())
                {
                    textures.push_back(this->textures[j]);
                    skip = true;

                    break;
                }
            }
            if (!skip)
            {
                Texture texture;
                std::string filePath = this->directory + '/' + str.C_Str();
                texture.LoadFromFile(filePath);
                texture.GetType() = typeName;
                std::cout << typeName << "\n";
                texture.GetPath() = str.C_Str();
                textures.push_back(texture);
                
                this->textures.push_back(texture);
            }
        }
        return textures;
    }
    void Model::Draw(Ref<Shader> &shader)
    {
        for (uint32_t i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(shader);
        }
    }
} // namespace Gecko