#include "gkpch.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Gecko
{
    Texture::Texture()
    {
        
    }
    void Texture::Bind(uint32_t slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }
    void Texture::UnBind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void Texture::LoadFromFile(std::string const &path)
    {
        m_Path = path;

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *data = stbi_load(m_Path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);

        if (data)
        {
            if (m_NrChannels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else if (m_NrChannels == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
            }
            m_TextureData = m_RendererID;
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            GK_LOG(GK_ERROR) << "Failed to load: " << BLUE << m_Path << RESET << " File does not exist!" << std::endl;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    Texture::~Texture()
    {
    }
}; // namespace Gecko