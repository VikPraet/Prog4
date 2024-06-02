#pragma once
#include "BaseComponent.h"
#include "ColliderComponent.h"
#include <iostream>

namespace dae
{
    class TestComponent : public BaseComponent, public EventListener
    {
    public:
        TestComponent(GameObject* gameObject);
        ~TestComponent() = default;

        void Update() override;

        void OnTriggerEnter(GameObject* self, GameObject* other);
        void OnTriggerExit(GameObject* self, GameObject* other);

    private:
        ColliderComponent* m_ColliderComponent;
    };
}
