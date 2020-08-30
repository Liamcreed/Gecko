#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Gecko
{
    class Model
    {
    public:
        std::vector<Ref<Material>> materials;
        std::vector<Ref<Mesh>> meshes;

        std::string directory;
        void LoadFromFile(std::string path);

    private:
        void ProcessNode(aiNode *node, const aiScene *scene);
        Ref<Mesh> ProcessMesh(aiMesh *mesh, const aiScene *scene);
        Ref<Material> ProcessMaterial(aiMesh *mesh, const aiScene *scene);
        Ref<Texture> LoadMaterialTexture(aiMaterial *mat, aiTextureType type,Ref<Material> &material);
    };
} // namespace Gecko
