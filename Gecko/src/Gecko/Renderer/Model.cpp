#include "gkpch.h"
#include "Model.h"
#include <assimp/pbrmaterial.h>

namespace Gecko
{
    void Model::LoadFromFile(std::string path)
    {
        //Clear meshes before loading new model
        meshes.clear();
        meshes.clear();

        materials.clear();

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            GK_LOG(BOLDRED "[MODEL LOADING ERROR]" RESET) << importer.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);
    }
    void Model::ProcessNode(aiNode *node, const aiScene *scene)
    {
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

            meshes.push_back(ProcessMesh(mesh, scene));
            materials.push_back(ProcessMaterial(mesh, scene));
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    Ref<Material> Model::ProcessMaterial(aiMesh *mesh, const aiScene *scene)
    {
        Ref<Material> mat = CreateRef<Material>();
        std::vector<Ref<Texture>> textures;

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Ref<Texture>> albedoMap = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "albedoMap", mat);
        textures.insert(textures.end(), albedoMap.begin(), albedoMap.end());

        std::vector<Ref<Texture>> metallicMap = LoadMaterialTexture(material, aiTextureType_UNKNOWN , "metallicMap", mat );
        /* if(metallicMap.size() == 0)
            std::exit(-1); */
        textures.insert(textures.end(), metallicMap.begin(), metallicMap.end());

        std::vector<Ref<Texture>> roughnessMap = LoadMaterialTexture(material, aiTextureType_UNKNOWN, "roughnessMap", mat);
        textures.insert(textures.end(),roughnessMap.begin(), roughnessMap.end());

        //std::vector<Ref<Texture>> normalMap = LoadMaterialTexture(material, aiTextureType_HEIGHT, "normalMap", mat);
        //textures.insert(textures.end(),normalMap.begin(), normalMap.end());

        //std::vector<Ref<Texture>> AOMap = LoadMaterialTexture(material, aiTextureType_AMBIENT, "AOMap", mat);
        //textures.insert(textures.end(),AOMap.begin(), AOMap.end());

        mat->SetTextures(textures);

        return mat;
    }
    Ref<Mesh> Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

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
        Ref<Mesh> aMesh = CreateRef<Mesh>(vertices, indices);

        return aMesh;
    }
    std::vector<Ref<Texture>> Model::LoadMaterialTexture(aiMaterial *mat, aiTextureType type, std::string typeName, Ref<Material> &material)
    {
        bool skip = false;
        std::vector<Ref<Texture>> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            for (uint32_t j = 0; j < material->GetTextures().size(); j++)
            {
                if (material->GetTextures()[j]->GetPath() == str.C_Str())
                {
                    textures.push_back(material->GetTextures()[j]);
                }
            }
            if (!skip)
            {
                Ref<Texture> texture = CreateRef<Texture>();
                std::string filePath = directory + '/' + str.C_Str();
                texture->LoadFromFile(filePath);

                texture->GetType() = typeName;
                texture->GetPath() = str.C_Str();

                textures.push_back(texture);
            }
        }
        return textures;
    }
} // namespace Gecko