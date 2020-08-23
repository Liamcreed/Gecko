#include "gkpch.h"
#include "Entity.h"
namespace Gecko
{
    Entity::Entity(entt::entity handle, Scene *scene)
        : m_Entityhandle(handle), m_Scene(scene)
    {
    }
} // namespace Gecko
