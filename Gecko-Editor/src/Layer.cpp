#include "gkpch.h"
#include "Layer.h"
#include <nfd.h>
#include "Panels/SceneHierarchyPanel.h"

namespace Gecko
{
    class CameraController : public EntityBehaviour
    {
    public:
        void Awake()
        {
        }
        void Update(DeltaTime dt)
        {
            auto &transform = GetComponent<TransformComponent>().Transform;
            float speed = 3.0f;
            transform[3][0] += Input::GetAxis("Horizontal") * speed * dt;
            transform[3][2] -= Input::GetAxis("Vertical") * speed * dt;
        }
    };

    Ref<Model> model;       //FIXME:
    Ref<Material> material; //FIXME:

    void EditorLayer::OnAttach()
    {
        model = CreateRef<Model>();
        model->LoadFromFile("assets/models/Roboteye/untitled.fbx");

        m_PlayerEntity = m_ActiveScene->CreateEntity("Player");
        m_PlayerEntity.AddComponent<MeshRendererComponent>(model->meshes[0], model->materials[0]); //FIXME:
        material = model->materials[0];                                                            //FIXME:

        m_MainCameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_MainCameraEntity.AddComponent<CameraComponent>();
        m_MainCameraEntity.AddComponent<ScriptComponent>().Bind<CameraController>();

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        Renderer::Init();
    }

    void EditorLayer::OnUpdate(DeltaTime dt)
    {
        m_Framebuffer.Bind();
        Renderer::Clear();

        m_ActiveScene->OnUpdate(dt);

        m_Framebuffer.UnBind();
    }

    void EditorLayer::OnImGuiRender()
    {
        ImGuiIO &io = ImGui::GetIO();

        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        //-----------------------DockSpace------------------------//
        ImGui::Begin("Gecko-Editor", NULL, window_flags);
        ImGui::PopStyleVar();
        if (opt_fullscreen)
            ImGui::PopStyleVar(2);
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        //-----------------------MenuBar------------------------//
        ImGui::GetStyle().WindowPadding = ImVec2(10.0f, 10.0f);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                    Application::Get()->Close();
                ImGui::EndMenu();
            }
        }
        

        ImGui::ShowDemoWindow();
        ImGui::Begin("Performance");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        static std::vector<float> fpsData(1000);
        static float lastTime = 0;
        if ((float)glfwGetTime() - lastTime >= 1.0 / 10)
        {
            lastTime = (float)glfwGetTime();
            fpsData.push_back(io.Framerate);
            fpsData.erase(fpsData.begin());
        }
        ImGui::PlotLines("Framerate", fpsData.data(), (int)fpsData.size(), 0, "",
                         0, 500, {250, 30});
        ImGui::Separator();
        ImGui::End();

        m_SceneHierarchyPanel.OnImGuiRenderer();

        ImGui::Begin("Properties");

        ImGui::DragFloat3("Camera Position",
                          &m_MainCameraEntity.GetComponent<TransformComponent>().Position.x, 0.01f);
        ImGui::DragFloat3("Cameaera Rotation",
                          &m_MainCameraEntity.GetComponent<TransformComponent>().Rotation.x, 0.1f);
        //FIXME: this is all temporary
        if (m_PlayerEntity)
        {
            ImGui::Separator();

            if (ImGui::Button("..."))
            {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog("fbx,obj,blend", NULL, &outPath);

                if (result == NFD_OKAY)
                {
                    model->LoadFromFile(outPath);
                    material = model->materials[0];
                    m_PlayerEntity.GetComponent<MeshRendererComponent>().material = material;

                    m_PlayerEntity.GetComponent<MeshRendererComponent>().mesh = model->meshes[0];
                }
            }
            ImGui::DragFloat3("player Position",
                              &m_PlayerEntity.GetComponent<TransformComponent>().Position.x, 0.01f);
            ImGui::DragFloat3("player Rotation",
                              &m_PlayerEntity.GetComponent<TransformComponent>().Rotation.x, 0.1f);
            ImGui::DragFloat3("player Scale",
                              &m_PlayerEntity.GetComponent<TransformComponent>().Size.x, 0.01f);

            if (ImGui::ImageButton((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetAlbedoMap()->GetTextureData(), ImVec2(30, 30)))
            {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog("png,jpg", NULL, &outPath);

                if (result == NFD_OKAY)
                {
                    Ref<Texture> tex = CreateRef<Texture>();
                    tex->LoadFromFile(outPath);
                    material->SetAlbedoMap(tex);
                }
            }

            ImGui::SameLine();
            ImGui::Text("Albedo");
            ImGui::SameLine();
            ImGui::ColorEdit3("Albedo", &material->GetAlbedo().x);
            m_PlayerEntity.GetComponent<MeshRendererComponent>().material = material;

            if (ImGui::ImageButton((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetMetallicMap()->GetTextureData(), ImVec2(30, 30)))
            {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog("png,jpg", NULL, &outPath);

                if (result == NFD_OKAY)
                {
                    Ref<Texture> tex = CreateRef<Texture>();
                    tex->LoadFromFile(outPath);
                    material->SetMetallicMap(tex);
                    m_PlayerEntity.GetComponent<MeshRendererComponent>().material = material;
                }
            }
            ImGui::SameLine();
            ImGui::DragFloat("Metallic", &material->GetRoughness(), 0.01f);

            ImGui::ImageButton((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetRoughnessMap()->GetTextureData(), ImVec2(30, 30));
            ImGui::SameLine();
            ImGui::DragFloat("Roughness", &material->GetMetallic(), 0.01f);

            ImGui::ImageButton((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetAOMap()->GetTextureData(), ImVec2(30, 30));
            ImGui::SameLine();
            ImGui::Text("ao");

            ImGui::Separator();
        }

        ImGui::End();

        //-----Viewport-----//
        ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
        ImGui::Begin("Scene");
        m_ViewFocused = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();
        ImGui::Image((void *)m_Framebuffer.GetColorAttachmentRendererID(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
        static ImVec2 lastSize;
        if ((lastSize.x != ImGui::GetWindowSize().x || lastSize.y != ImGui::GetWindowSize().y))
        {
            m_Framebuffer.Resize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
            m_ActiveScene->OnViewportResize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        }

        lastSize = ImGui::GetWindowSize();
        ImGui::End();

        ImGui::End();
        //-------------------//
    }
    void EditorLayer::OnDetach()
    {
    }
} // namespace Gecko