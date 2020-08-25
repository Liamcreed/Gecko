#pragma once
#include <glm/glm.hpp>
#include "Gecko/Renderer/Model.h"
#include "Gecko/Renderer/Camera.h"
#include "Gecko/Core/DeltaTime.h"
#include "SceneCamera.h"
#include "EntityBehaviour.h"
namespace Gecko
{
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent &) = default;
        TagComponent(const std::string &tag)
            : Tag(tag) {}
    };
    struct TransformComponent
    {
        glm::vec3 Size = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::mat4 Transform = glm::mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent &) = default;
        TransformComponent(const glm::mat4 &transform)
            : Transform(transform) {}

        operator glm::mat4 &() { return Transform; }
        operator const glm::mat4 &() const { return Transform; }
    };
    struct MeshRendererComponent
    {
        MeshRendererComponent() = default;
        MeshRendererComponent(const MeshRendererComponent &) = default;
        MeshRendererComponent(Ref<Mesh>& mesh, Ref<Material>& material) 
        {this->mesh = mesh; this->material =material; }

        Ref<Mesh> mesh;
        Ref<Material> material;
    };
    enum LightType
    {
        Sun = 0,
        PointLight = 1,
        SpotLight = 2
    };
    struct LightComponent
    {
        LightComponent() = default;
        LightComponent(const LightComponent &) = default;
        LightComponent(LightType type)
            : Type(type) {}

        LightType Type = Sun;
        float Intensity = 1.0f;
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
    };
    struct CameraComponent
    {
        SceneCamera camera;
        bool Primary = true; //TODO: leave this up to the scene
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent &) = default;
    };
    struct ScriptComponent
    {
        EntityBehaviour *Instance = nullptr;

        EntityBehaviour *(*InstantiateScript)();
        void (*DestroyScript)(ScriptComponent *);

        template <typename T>
        void Bind()
        {
            InstantiateScript = []() { return static_cast<EntityBehaviour *>(new T()); };
            DestroyScript = [](ScriptComponent *sc) {delete sc->Instance; sc->Instance = nullptr; };
        }
    };

} // namespace Gecko
