#pragma once
#include <glad/glad.h>

namespace Gecko
{
    class Skybox
    {
    private:
        uint32_t m_RendererID;
        int m_Width, m_Height, m_NrChannels;
        std::vector<std::string> m_Faces;

    public:
        Skybox(std::vector<std::string> faces);
        void Bind(uint32_t slot = 0);
        void UnBind();
        ~Skybox();
    };
} // namespace Gecko
