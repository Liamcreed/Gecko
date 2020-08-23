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
        Model(){}
        void Draw(Ref<Shader> &shader);
        std::vector<Texture> textures; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Mesh> meshes;
        std::string directory;
        void LoadFromFile(std::string path);

    private:
        void ProcessNode(aiNode *node, const aiScene *scene);
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    };
} // namespace Gecko
