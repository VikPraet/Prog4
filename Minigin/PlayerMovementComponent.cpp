#include "PlayerMovementComponent.h"

#include "GameObject.h"
#include "GameTime.h"

PlayerMovementComponent::PlayerMovementComponent(const std::shared_ptr<dae::GameObject>& gameObject, float speed)
	: BaseComponent(gameObject), m_TransformComponent(nullptr), m_Speed(speed)
{
	m_TransformComponent = GetGameObject()->GetComponent<TransformComponent>();
}

void PlayerMovementComponent::MoveLeft()
{
	if (m_TransformComponent) m_TransformComponent->Translate(-m_Speed * GameTime::GetInstance().GetDeltaTime(), 0, 0);
}

void PlayerMovementComponent::MoveRight()
{
	if (m_TransformComponent) m_TransformComponent->Translate(m_Speed * GameTime::GetInstance().GetDeltaTime(), 0, 0);
}

void PlayerMovementComponent::MoveUp()
{
	if (m_TransformComponent) m_TransformComponent->Translate(0, -m_Speed * GameTime::GetInstance().GetDeltaTime(), 0);
}

void PlayerMovementComponent::MoveDown()
{
	if (m_TransformComponent) m_TransformComponent->Translate(0, m_Speed * GameTime::GetInstance().GetDeltaTime(), 0);
}
