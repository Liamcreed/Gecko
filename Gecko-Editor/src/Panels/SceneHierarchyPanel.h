#pragma once
#include "Gecko/Scene/Scene.h"
#include "Gecko/Core/Core.h"
#include "Gecko/Scene/Entity.h"
#include "Gecko/Renderer/Texture.h"

namespace Gecko
{
    class SceneHierarchyPanel
    {
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;

        Ref<Texture> m_CubeTexture = CreateRef<Texture>();
    public:
        SceneHierarchyPanel(const Ref<Scene>& context);
        SceneHierarchyPanel(){}

        void SetContext(const Ref<Scene>& context);
        void OnImGuiRenderer();
    private:
        void DrawEntityNode(Entity entity);
    
    };

} // namespace Gecko
