#pragma once
#include "Shader.h"
#include "Texture.h"
#include "gkpch.h"
#include <glm/glm.hpp>
namespace Gecko
{
    class Material
    {
    private:
        Ref<Shader> m_Shader;
        glm::vec3 m_Albedo = glm::vec3(1.0f,1.0f,1.0f);
        float m_Metallic = 1.0f;
        float m_Roughness = 1.0f;

        

        Ref<Texture> m_AlbedoMap = CreateRef<Texture>();
        Ref<Texture> m_MetallicMap = CreateRef<Texture>();
        Ref<Texture> m_RoughnessMap = CreateRef<Texture>();
        Ref<Texture> m_NormalMap = CreateRef<Texture>();
        Ref<Texture> m_AOMap = CreateRef<Texture>();

    public:
        Material(){}
        ~Material();

        void Bind(Ref<Shader>& shader);
        void UnBind();
        
        void SetAlbedoMap(Ref<Texture>& texture) { m_AlbedoMap = texture; }
        void SetMetallicMap(Ref<Texture>& texture) { m_MetallicMap = texture; }
        void SetRoughnessMap(Ref<Texture>& texture) { m_RoughnessMap = texture; }
        void SetAOMap(Ref<Texture>& texture) { m_AOMap = texture; }
        void SetNormalMap(Ref<Texture>& texture) { m_NormalMap = texture; }

        

        void SetAlbedo(glm::vec3 color){m_Albedo = color;}
        void SetRoughness(float value){m_Roughness = value; }
        void SetMetallic(float value){m_Metallic = value; }
        
        void SetShader(Ref<Shader> shader){m_Shader = shader; }

        Ref<Texture> &GetAlbedoMap() {return m_AlbedoMap; }
        Ref<Texture> &GetMetallicMap() {return  m_MetallicMap;}
        Ref<Texture> &GetRoughnessMap() {return m_RoughnessMap; }
        Ref<Texture> &GetNormalMap() {return m_NormalMap; }
        Ref<Texture> &GetAOMap() {return m_AOMap; }

        glm::vec3 &GetAlbedo() {return m_Albedo; }
        float &GetMetallic() {return  m_Metallic;}
        float &GetRoughness() {return m_Roughness; }
        

        std::vector<Ref<Texture>> GetTextures(){return std::vector<Ref<Texture>> 
        {
            m_AlbedoMap,
            m_MetallicMap,
            m_RoughnessMap,
            m_NormalMap,
            m_AOMap
        };}
    };    
} // namespace Gecko
