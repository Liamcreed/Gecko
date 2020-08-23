#pragma once
#include "Gecko.h"

class EditorLayer : public Gecko::Layer
{
private:
    PerspectiveCameraController m_CamController{1280, 720};
    FrameBuffer m_Framebuffer{1440, 900};

    Ref<Scene> m_ActiveScene =  CreateRef<Scene>();
    Entity m_PlayerEntity;
    Entity m_MainCameraEntity;
    Entity m_SecondCameraEntity;

    bool m_PrimaryCamera  = true;
    bool m_ViewFocused;
public:
    EditorLayer(){}
    void OnAttach() override;

    void OnUpdate(DeltaTime dt) override;
    void OnImGuiRender() override;
    void OnDetach() override;
};
