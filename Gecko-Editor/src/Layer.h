#pragma once
#include "Gecko.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Gecko
{

    class EditorLayer : public Gecko::Layer
    {
    private:
        FrameBuffer m_Framebuffer{1440, 900};

        Ref<Scene> m_ActiveScene = CreateRef<Scene>();
        Entity m_PlayerEntity;
        Entity m_MainCameraEntity;

        bool m_ViewFocused;

        //Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
        
    public:
        EditorLayer() {}
        void OnAttach() override;

        void OnUpdate(DeltaTime dt) override;
        void OnImGuiRender() override;
        void OnDetach() override;
    };
} // namespace Gecko