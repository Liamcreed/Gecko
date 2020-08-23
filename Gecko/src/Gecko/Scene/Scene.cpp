#include "gkpch.h"
#include "glm/glm.hpp"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "Gecko/Renderer/Renderer.h"
#include "Gecko/Core/Input.h"

namespace Gecko
{
    Scene::Scene()
    {
        shader = CreateRef<Shader>("assets/shaders/default.vert.shader", "assets/shaders/default.frag.shader");
    }

    Entity Scene::CreateEntity(const std::string &name)
    {
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<TagComponent>(name);
        return entity;
    }

    void Scene::OnUpdate(DeltaTime dt)
    {
        //----Update-scripts----/
        {
            m_Registry.view<ScriptComponent>().each([=](auto entity, auto &sc) {
                //TODO: move to Scene::OnPlay
                if (!sc.Instance)
                {
                    sc.Instance = sc.InstantiateScript();
                    sc.Instance->m_Entity = {entity, this};
                    sc.Instance->Awake();
                }
                sc.Instance->Update(dt);
            });
        }

        //----------Rendering-----------//
        Camera *mainCamera = nullptr;
        glm::mat4 *cameraTransform = nullptr;
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera = &camera.camera;
                    cameraTransform = &transform.Transform;
                    break;
                }
            }
        }
        {
            //Lights
            auto view = m_Registry.view<TransformComponent, LightComponent>();
            for (auto entity : view)
            {
                auto [transform, light] = view.get<TransformComponent, LightComponent>(entity);
            }
        }

        //----Renderering-Models----//
        if (mainCamera)
        {
            Renderer::Submit(shader);
            Renderer::BeginScene(*mainCamera, *cameraTransform);
            {
                auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
                for (auto entity : group)
                {
                    auto [transform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);

                    Renderer::DrawModel(mesh.model, transform.Position, transform.Size, transform.Rotation);
                }
            }
            Renderer::EndScene();
        }
    }
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = width;

        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto &cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.camera.SetViewportSize(width, height);
            }
            //TODO:
            //Resize  fixed aspect ratio cameras
        }
    }

    Scene::~Scene()
    {
    }
} // namespace Gecko