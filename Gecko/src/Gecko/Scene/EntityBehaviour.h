#pragma once
#include "Entity.h"
namespace Gecko
{
    class EntityBehaviour
    {
    private:
        Entity m_Entity;
        friend class Scene;

    protected:
        virtual void Awake() {}
        virtual void Update(DeltaTime dt) {}
        virtual void Destruction() {}

    public:
        virtual ~EntityBehaviour() {}
        template <typename T>
        T &GetComponent()
        {
            if (!HasComponent<T>())
                GK_LOG(GK_WARNING) << "Entity does not have component!" << std::endl;
            return m_Entity.GetComponent<T>();
        }

        template <typename T>
        bool HasComponent()
        {
            return m_Entity.HasComponent<T>();
        }
    };
} // namespace Gecko
