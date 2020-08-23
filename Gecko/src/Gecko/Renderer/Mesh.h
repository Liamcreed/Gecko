#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Shader.h"
#include "Material.h"

namespace Gecko
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    class Mesh
    {
    private:
        uint32_t vao, vbo, ibo;

    public:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Texture> textures;
        
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
        void Draw(Ref<Shader> &shader);

    private:
        void CreateMesh();
    };

} // namespace Gecko
