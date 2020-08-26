#include "gkpch.h"
#include "Layer.h"
#include <nfd.h>
#include "Panels/SceneHierarchyPanel.h"

//------------Scripts-------------//
class PlayerMovement : public EntityBehaviour
{
public:
    void Awake()
    {
    }
    void Update(DeltaTime dt)
    {
        auto &transform = GetComponent<TransformComponent>();
        float speed = 6.0f;

        if (Input::KeyPressed(GK_KEY_R))
            transform.Rotation.y += speed * dt;
    }
};
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
//--------------------------------//

Ref<Model> caveTrollModel; //FIXME:

void EditorLayer::OnAttach()
{
    caveTrollModel = CreateRef<Model>();
    caveTrollModel->LoadFromFile("assets/models/Cavetroll/Cavetroll.fbx");

    m_PlayerEntity = m_ActiveScene->CreateEntity("Droid");
    m_PlayerEntity.AddComponent<MeshRendererComponent>(caveTrollModel->meshes[0],caveTrollModel->materials[0] );
    m_PlayerEntity.AddComponent<ScriptComponent>().Bind<PlayerMovement>();

    m_MainCameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
    m_MainCameraEntity.AddComponent<CameraComponent>();
    m_MainCameraEntity.AddComponent<ScriptComponent>().Bind<CameraController>();

    m_SecondCameraEntity = m_ActiveScene->CreateEntity("Second Camera Entity");
    auto &cc = m_SecondCameraEntity.AddComponent<CameraComponent>();
    cc.Primary = false;

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
    ImGui::Begin("DockSpace Demo", NULL, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::GetStyle().WindowPadding = ImVec2(10.0f, 10.0f);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                Application::Get()->Close();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    //----Performance----//
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
    //-------------------//

    m_SceneHierarchyPanel.OnImGuiRenderer();

    ImGui::ShowDemoWindow();

    ImGui::Begin("Properties");
    static bool vsync = true;
    ImGui::Checkbox("Vsync", &vsync);
    Application::Get()->GetWindow()->Vsync(vsync);

    ImGui::DragFloat3("Camera Transform",
                      glm::value_ptr(m_MainCameraEntity.GetComponent<TransformComponent>().Transform[3]), 0.01f);
    ImGui::DragFloat3("2nd  Camera Transform",
                      glm::value_ptr(m_SecondCameraEntity.GetComponent<TransformComponent>().Transform[3]), 0.01f);

    if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
    {
        m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
        m_MainCameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
    }
    {
        auto &camera = m_SecondCameraEntity.GetComponent<CameraComponent>().camera;
        float fov = camera.GetPerspectiveFOV();
        if (ImGui::DragFloat("2nd Camera FOV", &fov))
            camera.SetPerspectiveFOV(fov);
    }
    ImGui::Separator();

    //---------------------------//
    if (m_PlayerEntity)
    {
        ImGui::Separator();

        auto &tag = m_PlayerEntity.GetComponent<TagComponent>().Tag;
        ImGui::Text("%s", tag.c_str());

        if (ImGui::Button("..."))
        {
            nfdchar_t *outPath = NULL;
            nfdresult_t result = NFD_OpenDialog("obj,fbx", NULL, &outPath);

            if (result == NFD_OKAY)
            {
                caveTrollModel->LoadFromFile(outPath);
                m_PlayerEntity.GetComponent<MeshRendererComponent>().material = caveTrollModel->materials[0];
                m_PlayerEntity.GetComponent<MeshRendererComponent>().mesh = caveTrollModel->meshes[0];
            }
        }
        ImGui::DragFloat3("player Transform",
                          &m_PlayerEntity.GetComponent<TransformComponent>().Position.x, 0.01f);
        if (ImGui::TreeNode("Model textures"))
        {
            for (int i = 0; i < m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetTextures().size(); i++)
            {
                std::string name = m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetTextures()[i]->GetType() + " " + m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetTextures()[i]->GetPath();
                if (ImGui::TreeNode(name.c_str()))
                {
                    ImGui::Image((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetTextures()[i]->GetTextureData(),
                                 ImVec2(200, 200));
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
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
        m_CamController.OnResize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        m_Framebuffer.Resize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

        m_ActiveScene->OnViewportResize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    }
    lastSize = ImGui::GetWindowSize();
    ImGui::End();
    ImGui::End();
    //-----------------------------------------------------------//
}
void EditorLayer::OnDetach()
{
}