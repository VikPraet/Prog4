#include "TestCollisionComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

dae::TestComponent::TestComponent(GameObject* gameObject)
    : BaseComponent(gameObject), m_ColliderComponent(nullptr)
{
}

void dae::TestComponent::Update()
{
    if (!m_ColliderComponent)
    {
        m_ColliderComponent = GetGameObject()->GetComponent<ColliderComponent>();
        if (m_ColliderComponent)
        {
            m_ColliderComponent->OnTriggerEnterEvent.AddListener(this, &TestComponent::OnTriggerEnter);
            m_ColliderComponent->OnTriggerExitEvent.AddListener(this, &TestComponent::OnTriggerExit);
        }
    }
}

void dae::TestComponent::OnTriggerEnter(GameObject* self, GameObject* other)
{
    std::cout << "OnTriggerEnter: " << self->GetTag() << " with " << other->GetTag() << "\n";
}

void dae::TestComponent::OnTriggerExit(GameObject* self, GameObject* other)
{
    std::cout << "OnTriggerExit: " << self->GetTag() << " with " << other->GetTag() << "\n";
}
