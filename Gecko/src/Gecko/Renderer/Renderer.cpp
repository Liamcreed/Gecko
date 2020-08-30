#include "gkpch.h"
#include "Renderer.h"

namespace Gecko
{
    struct RendererData
    {
        glm::mat4 ProjectionMatrix;
        glm::mat4 TransformMatrix;
        glm::mat4 ViewMatrix;
        Ref<Shader> shader;
    };
    static RendererData s_Data;

    Renderer::Renderer()
    {
    }
    void Renderer::Init()
    {
        Application::Get()->GetWindow()->Vsync(true);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    void Renderer::Clear(glm::vec4 color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void Renderer::BeginScene(Camera &camera, const glm::mat4 &transform)
    {
        glm::mat4 view = glm::inverse(transform);
        glm::mat4 proj = camera.GetProjection();

        s_Data.ProjectionMatrix = proj;
        s_Data.ViewMatrix = view;

        s_Data.shader->Bind();
        s_Data.shader->SetMat4("projection", proj);
        s_Data.shader->SetMat4("view", view);
    }

    void Renderer::BeginScene(PerspectiveCamera &camera)
    {
        s_Data.ProjectionMatrix = camera.GetProjectionMatrix();
        s_Data.ViewMatrix = camera.GetViewMatrix();

        s_Data.shader->Bind();
        s_Data.shader->SetMat4("projection", camera.GetProjectionMatrix());
        s_Data.shader->SetMat4("view", camera.GetViewMatrix());
    }
    void Renderer::EndScene()
    {
    }
    void Renderer::Submit(const Ref<Shader> &shader, const glm::mat4 transform)
    {
        s_Data.shader = shader;
        s_Data.TransformMatrix = transform;
    }
    
    void Renderer::DrawMesh(Ref<Mesh>& mesh,Ref<Material>& material,const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation )
    {
        //Take in material
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::scale(transform, scale);
        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        DrawMesh(mesh,material,transform);
    }
    void Renderer::DrawMesh(Ref<Mesh>& mesh,Ref<Material>& material, const glm::mat4& transform)
    {
        s_Data.shader->SetMat4("model", transform);

        material->Bind(s_Data.shader);
        mesh->Bind();
        material->UnBind();
    }
    

    Renderer::~Renderer()
    {
    }
} // namespace Gecko
