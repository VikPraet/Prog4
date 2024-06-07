#include "Bullet.h"
#include "GameObject.h"
#include "GameTime.h"

galaga::Bullet::Bullet(dae::GameObject* gameObject, float speed, float lifetime)
    : BaseComponent(gameObject), m_Direction(0, -1), m_Speed(speed), m_Lifetime(lifetime), m_TransformComponent(nullptr)
{
}

void galaga::Bullet::Update()
{
    if(!m_TransformComponent) {
        m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();
    }
    else if(m_TransformComponent) {
        m_TransformComponent->Translate(m_Direction.x * m_Speed * dae::GameTime::GetInstance().GetDeltaTime(), m_Direction.y * m_Speed * dae::GameTime::GetInstance().GetDeltaTime(), 0);
    }

    const float deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
    m_TransformComponent->Translate(m_Direction.x * m_Speed * deltaTime, m_Direction.y * m_Speed * deltaTime, 0);
    m_Lifetime -= deltaTime;
    if (m_Lifetime <= 0.0f)
    {
        GetGameObject()->Destroy();
    }
}

void galaga::Bullet::SetDirection(const glm::vec2& direction)
{
    m_Direction = direction;
}

void galaga::Bullet::SetSpeed(float speed)
{
    m_Speed = speed;
}
