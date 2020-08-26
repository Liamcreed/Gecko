#pragma once
#include "Core.h"
#include "Window.h"
#include "Layer.h"
#include "DeltaTime.h"

#include "Gecko/ImGui/ImGuiLayer.h"


namespace Gecko
{
    class Application
    {
    private:
        Window *m_Window;
        LayerStack m_LayerStack;
        ImGuiLayer *m_ImGuiLayer;   

        float m_LastFrameTime = 0;
        bool m_Running = false;
    private:
        bool Init();
    public:
        Application(const std::string &title = "Application");
        virtual ~Application();

        static Application *s_Instance;
        static Application *Get() { return s_Instance; }

        void PushLayer(Layer *layer)
        {
            m_LayerStack.PushLayer(layer);
            layer->OnAttach();
        }
        void PushOverlay(Layer *layer)
        {
            m_LayerStack.PushOverlay(layer);
            layer->OnAttach();
        }

        void Run();
        void Close();
    public:
        Window *GetWindow() { return m_Window; }
    };
    Application *CreateApplication();
} // namespace Gecko
