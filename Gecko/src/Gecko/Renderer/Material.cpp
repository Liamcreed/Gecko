#include "gkpch.h"
#include "Material.h"
namespace Gecko
{
    Material::~Material()
    {
    }
    void Material::Bind(Ref<Shader> &shader)
    {
        for (int i = 0; i < m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            shader->SetInt("material." + m_Textures[i]->GetType(), i);
            glBindTexture(GL_TEXTURE_2D, m_Textures[i]->GetRendererID());
        }
    }
    void Material::SetTextures(std::vector<Ref<Texture>> textures)
    {
        m_Textures = textures;
    }
    void Material::SetValue(std::string &name, float value)
    {
        m_Shader->SetFloat("material." + name, value);
    }
} // namespace Gecko
