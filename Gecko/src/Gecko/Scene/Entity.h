#pragma once
#include "entt/entt.hpp"

#include "Scene.h"

namespace Gecko
{
    class Entity
    {
    private:
        entt::entity m_Entityhandle{entt::null};
        Scene *m_Scene = nullptr;

    public:
        Entity() = default;
        Entity(entt::entity handle, Scene *scene);
        Entity(const Entity &other) = default;
        ~Entity() {}

        template <typename T, typename... Args>
        T &AddComponent(Args &&... args)
        {
            if (HasComponent<T>())
                GK_LOG(GK_WARNING) << "Entity already has component!" << std::endl;

            return m_Scene->m_Registry.emplace<T>(m_Entityhandle, std::forward<Args>(args)...);
        }

        template <typename T>
        T &GetComponent()
        {
            if (!HasComponent<T>())
                GK_LOG(GK_WARNING) << "Entity does not have component!" << std::endl;
            return m_Scene->m_Registry.get<T>(m_Entityhandle);
        }

        template <typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.has<T>(m_Entityhandle);
        }

        template <typename T>
        void RemoveComponent()
        {
            if (!HasComponent<T>())
                GK_LOG(GK_WARNING) << "Entity does not have component!" << std::endl;
            else
                return m_Scene->m_Registry.remove<T>(m_Entityhandle);
            return;
        }
        operator bool() const { return m_Entityhandle != entt::null; }
    };

} // namespace Gecko
