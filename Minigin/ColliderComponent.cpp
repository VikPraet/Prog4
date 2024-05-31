#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

dae::ColliderComponent::ColliderComponent(GameObject* gameObject, const glm::vec2& size, bool isTrigger)
    : BaseComponent(gameObject), m_Size(size), m_IsTrigger(isTrigger)
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

bool dae::ColliderComponent::IsTrigger() const
{
    return m_IsTrigger;
}

bool dae::ColliderComponent::IsCollidingWith(const ColliderComponent* other) const
{
	const auto posA = GetGameObject()->GetComponent<TransformComponent>()->GetWorldPosition();
	const auto posB = other->GetGameObject()->GetComponent<TransformComponent>()->GetWorldPosition();
	const auto sizeA = GetSize();
	const auto sizeB = other->GetSize();

    return (posA.x < posB.x + sizeB.x &&
        posA.x + sizeA.x > posB.x &&
        posA.y < posB.y + sizeB.y &&
        posA.y + sizeA.y > posB.y);
}
