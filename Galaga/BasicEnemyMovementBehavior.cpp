#include "BasicEnemyMovementBehavior.h"
#include "GameObject.h"
#include "GameTime.h"

galaga::BasicEnemyMovementBehavior::BasicEnemyMovementBehavior(dae::GameObject* gameObject, float speed, glm::vec2 startPosition)
    : BaseComponent(gameObject), m_TransformComponent(nullptr), m_Speed(speed), m_MovingRight(true), m_CanMove(true)
{
    if (!m_TransformComponent)
        m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();

    m_MinX = 50.0f;
    m_MaxX = static_cast<float>(dae::Settings::window_width - 50);
    m_Position = glm::vec3(startPosition, 0.0f);
}

void galaga::BasicEnemyMovementBehavior::Update()
{
    if (!m_TransformComponent)
        m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();

    if (m_TransformComponent)
    {
        const float deltaTime = dae::GameTime::GetInstance().GetDeltaTime();

        if (m_MovingRight)
            m_Position.x += m_Speed * deltaTime;
        else
            m_Position.x -= m_Speed * deltaTime;

        KeepWithinBounds();

        if (m_CanMove)
        {
            m_TransformComponent->SetWorldPosition(m_Position);
        }
    }
}

void galaga::BasicEnemyMovementBehavior::SetBounds(float minX, float maxX)
{
    m_MinX = minX;
    m_MaxX = maxX;
}

void galaga::BasicEnemyMovementBehavior::KeepWithinBounds()
{
    if (m_Position.x < m_MinX)
    {
        m_Position.x = m_MinX;
        m_MovingRight = true;
    }
    if (m_Position.x > m_MaxX)
    {
        m_Position.x = m_MaxX;
        m_MovingRight = false;
    }
}

void galaga::BasicEnemyMovementBehavior::ResetPosition(const glm::vec2& position)
{
    m_Position = glm::vec3(position, 0);
    m_TransformComponent->SetWorldPosition(m_Position);
}
