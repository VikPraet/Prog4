#include "Bullet.h"
#include "GameObject.h"
#include "GameTime.h"
#include <glm/gtx/rotate_vector.hpp>
#include <cmath>

galaga::Bullet::Bullet(dae::GameObject* gameObject, float speed, float lifetime, const glm::vec2& direction)
    : BaseComponent(gameObject), m_TransformComponent(nullptr), m_Direction(direction), m_Speed(speed), m_Lifetime(lifetime)
{
    if (!m_TransformComponent) m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();
    UpdateRotation();
}

void galaga::Bullet::Update()
{
	const float deltaTime = dae::GameTime::GetInstance().GetDeltaTime();

    if (!m_TransformComponent) {
        m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();
        UpdateRotation();
    }
    else {
	    const glm::vec2 normalizedDirection = normalize(m_Direction);
        m_TransformComponent->Translate(normalizedDirection.x * m_Speed * deltaTime, normalizedDirection.y * m_Speed * deltaTime, 0);
    }

    m_Lifetime -= deltaTime;
    if (m_Lifetime <= 0.0f)
    {
        GetGameObject()->Destroy();
    }
}

void galaga::Bullet::SetDirection(const glm::vec2& direction)
{
    m_Direction = direction;
    UpdateRotation();
}

void galaga::Bullet::SetSpeed(float speed)
{
    m_Speed = speed;
}

void galaga::Bullet::UpdateRotation() const
{
    if (!m_TransformComponent) return;

    // Calculate the angle in degrees
    const float angle = std::atan2(m_Direction.y, m_Direction.x) * (180.0f / glm::pi<float>());
    m_TransformComponent->SetRotation(angle + 90.0f);
}
