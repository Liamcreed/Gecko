#include "gkpch.h"
#include "Application.h"

namespace Gecko
{
    Application *Application::s_Instance = nullptr;

    Application::Application(const std::string &title)
    {
        s_Instance = this;

        m_Window = new Window(title, 1280, 720);
        m_Window->SetTitle(title + "   |    OpenGL " + (const char*)glGetString( GL_VERSION ));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_Running = true;
    }
    
    bool Application::Init()
    {
        return true;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime();
            DeltaTime deltaTime = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (Layer *layer : m_LayerStack)
                layer->OnUpdate(deltaTime);

            /* glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT); */

            m_ImGuiLayer->Begin();
            for (Layer *layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->Update();
            if (m_Window->Closed())
                Close();
        }
    }
    void Application::Close()
    {
        m_Running = false;

        for (Layer *layer : m_LayerStack)
            layer->OnDetach();

        delete m_ImGuiLayer;
        delete m_Window;
    }
    Application::~Application()
    {
    }

} // namespace Gecko
