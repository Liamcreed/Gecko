#pragma once
#include <entt/entt.hpp>
#include <Gecko/Renderer/Shader.h>
namespace Gecko
{
    class Entity;

    class Scene
    {
    private:
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string &name = std::string());

        void OnUpdate(DeltaTime dt);
        void OnViewportResize(uint32_t width, uint32_t height);

    private:
        Ref<Shader> shader;
        
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        friend class SceneHierarchyPanel;
    };
} // namespace Gecko
