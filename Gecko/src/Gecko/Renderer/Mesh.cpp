#include "gkpch.h"
#include "Mesh.h"

namespace Gecko
{
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        CreateMesh();
    }
    void Mesh::Draw(Ref<Shader> &shader)
    {
        for (uint32_t i = 0; i < textures.size(); i++)
        {
            std::string name = textures[i].GetType();
            
            shader->SetFloat(("material." + name).c_str(), i);
        
            textures[i].Bind(i);
        }
        glActiveTexture(GL_TEXTURE0);

        shader->SetFloat("material.shininess",  32.0f);
        
        //Render mesh
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    void Mesh::CreateMesh()
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

        //Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        //Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        //Texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        
        glBindVertexArray(0);
    }

} // namespace Gecko