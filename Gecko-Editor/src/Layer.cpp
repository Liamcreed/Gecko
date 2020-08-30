#include "gkpch.h"
#include "Layer.h"
#include <nfd.h>
#include "Panels/SceneHierarchyPanel.h"

using namespace Gecko;

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

Ref<Model> model; //FIXME:

void EditorLayer::OnAttach()
{
    //Load model
    model = CreateRef<Model>();
    model->LoadFromFile("assets/models/Roboteye/untitled.fbx");

    //Creating material
    //FIXME:
    Ref<Material> mat = model->materials[0];

    m_PlayerEntity = m_ActiveScene->CreateEntity("Player");
    m_PlayerEntity.AddComponent<MeshRendererComponent>(model->meshes[0], mat); //FIXME:
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
    //FIXME: this is all temporary
    if (m_PlayerEntity)
    {
        ImGui::Separator();

        auto &tag = m_PlayerEntity.GetComponent<TagComponent>().Tag;
        ImGui::Text("%s", tag.c_str());

        if (ImGui::Button("..."))
        {
            nfdchar_t *outPath = NULL;
            nfdresult_t result = NFD_OpenDialog("fbx,obj", NULL, &outPath);

            if (result == NFD_OKAY)
            {
                model->LoadFromFile(outPath);
                m_PlayerEntity.GetComponent<MeshRendererComponent>().material = model->materials[0];
                m_PlayerEntity.GetComponent<MeshRendererComponent>().mesh = model->meshes[0];
            }
        }
        ImGui::DragFloat3("player Position",
                          &m_PlayerEntity.GetComponent<TransformComponent>().Position.x, 0.01f);
        ImGui::DragFloat3("player Rotation",
                          &m_PlayerEntity.GetComponent<TransformComponent>().Rotation.x, 0.1f);
        ImGui::DragFloat3("player Scale",
                          &m_PlayerEntity.GetComponent<TransformComponent>().Size.x, 0.01f);

        static Ref<Material> mat = CreateRef<Material>();

        if(ImGui::ImageButton((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetAlbedoMap()->GetTextureData() , ImVec2(50, 50)))
        {
            nfdchar_t *outPath = NULL;
            nfdresult_t result = NFD_OpenDialog("fbx,obj", NULL, &outPath);
            
            if (result == NFD_OKAY){Ref<Texture> tex = CreateRef<Texture>(); tex->LoadFromFile(outPath); mat->SetAlbedoMap(tex); m_PlayerEntity.GetComponent<MeshRendererComponent>().material = model->materials[0];}
            
        } 
        ImGui::SameLine(); ImGui::Text("Albedo");
        
        ImGui::ImageButton((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetMetallicMap()->GetTextureData() , ImVec2(50, 50));
        ImGui::SameLine(); ImGui::Text("Metallic");

        ImGui::ImageButton((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetRoughnessMap()->GetTextureData() , ImVec2(50, 50));
        ImGui::SameLine(); ImGui::Text("roughness");

        ImGui::ImageButton((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetNormalMap()->GetTextureData() , ImVec2(50, 50));
        ImGui::SameLine(); ImGui::Text("normal");

        ImGui::ImageButton((void *)m_PlayerEntity.GetComponent<MeshRendererComponent>().material->GetAOMap()->GetTextureData() , ImVec2(50, 50));
        ImGui::SameLine(); ImGui::Text("ao");

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