#include "gkpch.h"
#include "Model.h"
#include <assimp/pbrmaterial.h>

namespace Gecko
{
    void Model::LoadFromFile(std::string path)
    {
        //Clear meshes before loading new model
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

        Ref<Texture> albedoMap = LoadMaterialTexture(material, aiTextureType_DIFFUSE, mat);
        mat->SetAlbedoMap(albedoMap);

        aiColor4D albedo;
        if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &albedo))
            mat->SetAlbedo(glm::vec3(albedo.r, albedo.g, albedo.b));

        float roughness;
        if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &roughness))
            mat->SetRoughness(roughness);

        float metallic;
        if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_COLOR_REFLECTIVE, &metallic))
            mat->SetMetallic(metallic);

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
    Ref<Texture> Model::LoadMaterialTexture(aiMaterial *mat, aiTextureType type, Ref<Material> &material)
    {
        bool skip = false;
        Ref<Texture> texture = CreateRef<Texture>();

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, 0, &str);

            for (uint32_t j = 0; j < material->GetTextures().size(); j++)
            {
                if (material->GetTextures()[j]->GetPath() == directory + '/' + str.C_Str())
                {
                    return material->GetTextures()[j];
                }
            }

            std::string filePath = directory + '/' + str.C_Str();
            if (filePath != directory)
            {
                texture->LoadFromFile(filePath);
                texture->GetPath() = str.C_Str();
            }
        }

        return texture;
    }
} // namespace Gecko