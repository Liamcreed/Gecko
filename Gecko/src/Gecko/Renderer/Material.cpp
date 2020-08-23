#include "gkpch.h"
#include "Material.h"
namespace Gecko
{
    Material::Material(Ref<Shader> shader)
        : m_Shader(shader)
    {
    }

    Material::~Material()
    {
    }
    void Material::Bind()
    {
        m_Shader->SetFloat(("material." + m_AlbedoTexture->GetType()).c_str(), 0);
        m_AlbedoTexture->Bind();
        m_Shader->SetFloat(("material." + m_MetallicTexture->GetType()).c_str(), 0);
        m_MetallicTexture->Bind();
        m_Shader->SetFloat(("material." + m_RoughnessTexture->GetType()).c_str(), 0);
        m_RoughnessTexture->Bind();
        m_Shader->SetFloat(("material." + m_AOTexture->GetType()).c_str(), 0);
        m_AOTexture->Bind();
        m_Shader->SetFloat(("material." + m_NormalTexture->GetType()).c_str(), 0);
        m_NormalTexture->Bind();

        m_Shader->SetFloat("albedo", m_Albedo);
        m_Shader->SetFloat("roughness", m_Roughness);
        m_Shader->SetFloat("AO", m_AO);
        m_Shader->SetFloat("metallic", m_Metallic);
    }
    void Material::SetTexture(std::string &name, Ref<Texture> texture)
    {
        if (name == "albedo")
            m_AlbedoTexture = texture;
        if (name == "roughness")
            m_RoughnessTexture = texture;
        if (name == "metallic")
            m_MetallicTexture = texture;
        if (name == "ao" || name == "AO")
            m_NormalTexture = texture;
        if (name == "normal")
            m_NormalTexture = texture;
    }
    void Material::SetValue(std::string &name, float value)
    {
        m_Shader->SetFloat("material." + name, value);
    }
} // namespace Gecko
