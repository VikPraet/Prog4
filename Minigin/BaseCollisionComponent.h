#pragma once
#include <iostream>

#include "BaseComponent.h"

namespace dae
{
    class BaseCollisionComponent : public BaseComponent
    {
    public:
        BaseCollisionComponent(GameObject* gameObject);
        virtual ~BaseCollisionComponent() = default;

        void Update() override;

        virtual void OnCollisionEnter([[maybe_unused]] GameObject* other) { }
        virtual void OnCollisionExit([[maybe_unused]] GameObject* other) { }
        virtual void OnTriggerEnter([[maybe_unused]] GameObject* other) { }
        virtual void OnTriggerExit([[maybe_unused]] GameObject* other) { }

    private:
        void SubscribeToEvents();
        bool m_IsSubscribed{false};
    };
}
