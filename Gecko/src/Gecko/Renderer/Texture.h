#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>

namespace Gecko
{
    class Texture
    {
    private:
        uint32_t m_RendererID;
        int m_Width, m_Height, m_NrChannels;
        std::string m_Path;
        std::string m_Type;

        uint32_t m_TextureData;
    public:
        Texture();
        std::string &GetPath() { return m_Path; }
        std::string &GetType() { return m_Type; }
        uint32_t GetTextureData(){return m_TextureData;}
        glm::vec2 GetSize(){return glm::vec2(m_Width, m_Height);}

        void Bind(uint32_t slot = 0);
        void UnBind();
        void LoadFromFile(std::string const &path);
        ~Texture();
    };

} // namespace Gecko
