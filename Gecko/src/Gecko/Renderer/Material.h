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
        std::vector<Ref<Texture>> m_Textures;
    public:
        //FIXME:  std::vector<Ref<Texture>> instead of this 
        std::vector<Ref<Texture>> &GetTextures(){return m_Textures; }
        
        ~Material();

        void Bind(Ref<Shader>& shader);
        void SetTextures(std::vector<Ref<Texture>> textures);
        void SetValue(std::string& name, float value);
        void SetShader(Ref<Shader> shader){m_Shader = shader; }
    };    
} // namespace Gecko
