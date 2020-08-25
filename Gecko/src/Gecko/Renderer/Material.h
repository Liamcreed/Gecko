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
    public:
        Ref<Texture> AlbedoTexture;
        Ref<Texture> MetallicTexture;
        Ref<Texture> RoughnessTexture;
        Ref<Texture> NormalTexture;
        Ref<Texture> AOTexture;

        std::vector<Ref<Texture>> GetTextures(){return std::vector<Ref<Texture>>
        {
            AlbedoTexture,MetallicTexture,RoughnessTexture,
            NormalTexture, AOTexture
        };}
        
        ~Material();

        void Bind(Ref<Shader>& shader);
        void SetTexture(std::string name,Ref<Texture> texture);
        void SetValue(std::string& name, float value);
        void SetShader(Ref<Shader> shader){m_Shader = shader; }
    };    
} // namespace Gecko
