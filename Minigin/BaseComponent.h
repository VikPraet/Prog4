#pragma once
#include <memory>
#include "Event.h"

namespace dae
{
    class GameObject;

    class BaseComponent : public EventListener
    {
    public:
        BaseComponent(GameObject* gameObject);
        virtual ~BaseComponent() = default;

        virtual void FixedUpdate() {}
        virtual void Update() {}
        virtual void LateUpdate() {}
        virtual void Render() const {}

        virtual void OnComponentAdded([[maybe_unused]] BaseComponent* component) {}

        void SetActive(bool active) { m_IsActive = active; }
        bool GetActive() const { return m_IsActive; }

        void Destroy();
        bool IsMarkedForDestroy() const { return m_MarkedForDestroy; }

        GameObject* GetGameObject() const { return m_GameObject; }

    private:
        GameObject* m_GameObject;
        bool m_IsActive{ true };
        bool m_MarkedForDestroy{ false };
    };
}
