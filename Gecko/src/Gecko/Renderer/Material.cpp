#include "gkpch.h"
#include "Material.h"
namespace Gecko
{
    Material::~Material()
    {
    }
    void Material::Bind(Ref<Shader> &shader)
    {
        m_Shader = shader;

        //Albedo map
        glActiveTexture(GL_TEXTURE0);
        m_Shader->SetInt("material.albedoMap", 0);
        glBindTexture(GL_TEXTURE_2D, m_AlbedoMap->GetRendererID());

        //Metallic map
        glActiveTexture(GL_TEXTURE1);
        m_Shader->SetInt("material.metallicMap", 1);
        glBindTexture(GL_TEXTURE_2D, m_MetallicMap->GetRendererID());
        
        //Roughness Map map
        glActiveTexture(GL_TEXTURE2);
        m_Shader->SetInt("material.roughnessMap", 2);
        glBindTexture(GL_TEXTURE_2D, m_RoughnessMap->GetRendererID());

        //Normal map
        glActiveTexture(GL_TEXTURE3);
        m_Shader->SetInt("material.normalMap", 3);
        glBindTexture(GL_TEXTURE_2D, m_NormalMap->GetRendererID());

        //AO map
        glActiveTexture(GL_TEXTURE4);
        m_Shader->SetInt("material.AOMap", 4);
        glBindTexture(GL_TEXTURE_2D, m_AOMap->GetRendererID());
        
    
        m_Shader->SetVec3("material.albedo", m_Albedo.x, m_Albedo.y, m_Albedo.z);
        m_Shader->SetFloat("material.metallic", m_Metallic);
        m_Shader->SetFloat("material.roughness", m_Roughness);
    }
    void Material::UnBind()
    {
        m_AlbedoMap->UnBind();
        m_MetallicMap->UnBind();
        m_RoughnessMap->UnBind();
        m_NormalMap->UnBind();
        m_AOMap->UnBind();
    }
} // namespace Gecko
