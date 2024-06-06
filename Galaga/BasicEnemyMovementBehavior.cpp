#include "BasicEnemyMovementBehavior.h"
#include "GameObject.h"
#include "GameTime.h"

galaga::BasicEnemyMovementBehavior::BasicEnemyMovementBehavior(dae::GameObject* gameObject, float speed)
    : BaseComponent(gameObject), m_TransformComponent(nullptr), m_Speed(speed), m_MovingRight(true)
{
    m_MinX = 50.0f;
    m_MaxX = static_cast<float>(dae::Settings::window_width - 50);
}

void galaga::BasicEnemyMovementBehavior::Update()
{
    if (!m_TransformComponent)
        m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();

    if (m_TransformComponent)
    {
	    const float deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
        glm::vec3 position = m_TransformComponent->GetWorldPosition();

        if (m_MovingRight)
            position.x += m_Speed * deltaTime;
        else
            position.x -= m_Speed * deltaTime;

        m_TransformComponent->SetWorldPosition(position);
        KeepWithinBounds();
    }
}

void galaga::BasicEnemyMovementBehavior::SetBounds(float minX, float maxX)
{
    m_MinX = minX;
    m_MaxX = maxX;
}

void galaga::BasicEnemyMovementBehavior::KeepWithinBounds()
{
    auto pos = m_TransformComponent->GetWorldPosition();

    if (pos.x < m_MinX)
    {
        pos.x = m_MinX;
        m_MovingRight = true;
    }
    if (pos.x > m_MaxX)
    {
        pos.x = m_MaxX;
        m_MovingRight = false;
    }

    m_TransformComponent->SetWorldPosition(pos);
}
