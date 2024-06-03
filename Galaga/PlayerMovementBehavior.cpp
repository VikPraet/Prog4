#include "PlayerMovementBehavior.h"
#include "GameObject.h"
#include "GameTime.h"

galaga::PlayerMovementBehavior::PlayerMovementBehavior(dae::GameObject* gameObject)
    : BaseComponent(gameObject), m_Speed(50.0f), m_TransformComponent(nullptr)
{
    m_MinX = 50.f;
    m_MaxX = static_cast<float>(dae::Settings::window_width) - 50.f;
}

void galaga::PlayerMovementBehavior::Update()
{
	if(!m_TransformComponent) m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();
}

void galaga::PlayerMovementBehavior::Move(const glm::vec2& direction)
{
    m_TransformComponent->Translate(normalize(direction).x * m_Speed * dae::GameTime::GetInstance().GetDeltaTime(), 0, 0);
    KeepWithinBounds();
}

void galaga::PlayerMovementBehavior::SetBounds(float minX, float maxX)
{
    m_MinX = minX;
    m_MaxX = maxX;
}

void galaga::PlayerMovementBehavior::SetSpeed(float speed)
{
    m_Speed = speed;
}

void galaga::PlayerMovementBehavior::KeepWithinBounds()
{
    auto pos = m_TransformComponent->GetWorldPosition();

    if (pos.x < m_MinX) pos.x = m_MinX;
    if (pos.x > m_MaxX) pos.x = m_MaxX;

    m_TransformComponent->SetWorldPosition(pos);
}
