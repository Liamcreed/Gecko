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
        shader = CreateRef<Shader>("assets/shaders/default.vert", "assets/shaders/default.frag");
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
                auto [transformComponent, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera = &camera.camera;

                    glm::mat4 transform = glm::mat4(1.0f);
                    transform = glm::translate(transformComponent.Transform, transformComponent.Position);
                    transform = glm::rotate(transform, glm::radians(transformComponent.Rotation.x), glm::vec3(1, 0, 0));
                    transform = glm::rotate(transform, glm::radians(transformComponent.Rotation.y), glm::vec3(0, 1, 0));
                    transform = glm::rotate(transform, glm::radians(transformComponent.Rotation.z), glm::vec3(0, 0, 1));

                    cameraTransform = &transform;
                    break;
                }
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
                    auto [transform, meshRenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);
                    Renderer::DrawMesh(meshRenderer.mesh, meshRenderer.material, transform.Position, transform.Size, transform.Rotation);
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