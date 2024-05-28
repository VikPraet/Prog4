#pragma once
#include <memory>

namespace dae
{
    class GameObject;


    class BaseComponent
    {
    public:
        BaseComponent(dae::GameObject* gameObject);
        virtual ~BaseComponent() = default;

        virtual void FixedUpdate() {}
        virtual void Update() {}
        virtual void LateUpdate() {}
        virtual void Render() const {}

        void SetActive(bool active) { m_IsActive = active; }
        bool GetActive() const { return m_IsActive; }

        void Destroy();
        bool IsMarkedForDestroy() const { return m_MarkedForDestroy; }

    protected:
        dae::GameObject* GetGameObject() const { return m_GameObject; }

    private:
        dae::GameObject* m_GameObject;
        bool m_IsActive{ true };
        bool m_MarkedForDestroy{ false };
    };
}
