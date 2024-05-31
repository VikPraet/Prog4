#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

dae::ColliderComponent::ColliderComponent(GameObject* gameObject, const glm::vec2& size)
    : BaseComponent(gameObject), m_Size(size), m_TransformComponent(nullptr)
{
}

void dae::ColliderComponent::SetSize(const glm::vec2& size)
{
    m_Size = size;
}

const glm::vec2& dae::ColliderComponent::GetSize() const
{
    return m_Size;
}

bool dae::ColliderComponent::IsCollidingWith(const ColliderComponent* other)
{
    if (m_TransformComponent == nullptr)
        m_TransformComponent = GetGameObject()->GetComponent<TransformComponent>();

    if (other->m_TransformComponent == nullptr)
        other->m_TransformComponent = other->GetGameObject()->GetComponent<TransformComponent>();

    const glm::vec2 posA = m_TransformComponent->GetWorldPosition();
    const glm::vec2 posB = other->m_TransformComponent->GetWorldPosition();
    const glm::vec2 sizeA = GetSize();
    const glm::vec2 sizeB = other->GetSize();

    return (posA.x < posB.x + sizeB.x &&
            posA.x + sizeA.x > posB.x &&
            posA.y < posB.y + sizeB.y &&
            posA.y + sizeA.y > posB.y);
}
