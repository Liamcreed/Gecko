#include "gkpch.h"
#include "Material.h"
namespace Gecko
{
    Material::~Material()
    {
    }
    void Material::Bind(Ref<Shader>& shader)
    {
        AlbedoTexture->Bind(0);
        shader->SetFloat("material.albedoMap", 0);
    
        MetallicTexture->Bind(1);
        shader->SetFloat("material.metallicMap", 1);
        
        RoughnessTexture->Bind(2);
        shader->SetFloat("material.roughnessMap", 2);

        AOTexture->Bind(3);
        shader->SetFloat("material.AOMap", 3);
        
        NormalTexture->Bind(4);
        shader->SetFloat("material.normalMap", 4);
        

        shader->SetFloat("albedo", m_Albedo);
        shader->SetFloat("roughness", m_Roughness);
        shader->SetFloat("AO", m_AO);
        shader->SetFloat("metallic", m_Metallic);
    }
    void Material::SetTexture(std::string name, Ref<Texture> texture)
    {
        if (name == "albedo")
            AlbedoTexture = texture;
        if (name == "roughness")
            RoughnessTexture = texture;
        if (name == "metallic")
            MetallicTexture = texture;
        if (name == "ao" || name == "AO")
            AOTexture = texture;
        if (name == "normal")
            NormalTexture = texture;
    }
    void Material::SetValue(std::string &name, float value)
    {
        m_Shader->SetFloat("material." + name, value);
    }
} // namespace Gecko
