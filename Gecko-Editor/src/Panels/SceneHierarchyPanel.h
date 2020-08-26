#pragma once
#include "Gecko/Scene/Scene.h"
#include "Gecko/Core/Core.h"
#include "Gecko/Scene/Entity.h"
namespace Gecko
{
    class SceneHierarchyPanel
    {
    private:
        Ref<Scene> m_Context;
    public:
        SceneHierarchyPanel(const Ref<Scene>& context);
        SceneHierarchyPanel(){}

        void SetContext(const Ref<Scene>& context);
        void OnImGuiRenderer();
    private:
        void DrawEntityNode(Entity entity);
        Entity m_SelectionContext;
    };

} // namespace Gecko
