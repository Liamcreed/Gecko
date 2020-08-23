#pragma once
#include "Shader.h"
#include "Texture.h"
#include "gkpch.h"
namespace Gecko
{
    class Material
    {
    private:
        Ref<Shader> m_Shader;
        float m_Albedo;
        float m_Metallic;
        float m_Roughness;
        float m_AO;
        Ref<Texture> m_AlbedoTexture;
        Ref<Texture> m_MetallicTexture;
        Ref<Texture> m_RoughnessTexture;
        Ref<Texture> m_NormalTexture;
        Ref<Texture> m_AOTexture;
    public:
        Material(Ref<Shader> shader);
        ~Material();

        void Bind();
        void SetTexture(std::string &name,Ref<Texture> texture);
        void SetValue(std::string& name, float value);
        void SetShader(Ref<Shader> shader){m_Shader = shader; }
    };    
} // namespace Gecko
