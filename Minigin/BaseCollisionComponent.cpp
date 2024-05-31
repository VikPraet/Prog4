#include "BaseCollisionComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"

dae::BaseCollisionComponent::BaseCollisionComponent(GameObject* gameObject)
    : BaseComponent(gameObject)
{
}

void dae::BaseCollisionComponent::Update()
{
	if(!m_IsSubscribed)
	{
        SubscribeToEvents();
        m_IsSubscribed = true;
	}
}

void dae::BaseCollisionComponent::SubscribeToEvents()
{
    auto rigidbody = GetGameObject()->GetComponent<RigidbodyComponent>();
    if (rigidbody)
    {
        rigidbody->OnCollisionEnterEvent.addListener([this]([[maybe_unused]] GameObject* self, GameObject* other) { OnCollisionEnter(other); });
        rigidbody->OnCollisionExitEvent.addListener([this]([[maybe_unused]] GameObject* self, GameObject* other) { OnCollisionExit(other); });
        rigidbody->OnTriggerEnterEvent.addListener([this]([[maybe_unused]] GameObject* self, GameObject* other) { OnTriggerEnter(other); });
        rigidbody->OnTriggerExitEvent.addListener([this]([[maybe_unused]] GameObject* self, GameObject* other) { OnTriggerExit(other); });
    }
}
