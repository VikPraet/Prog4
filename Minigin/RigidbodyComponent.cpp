#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "SceneManager.h"
#include "Settings.h"

dae::RigidbodyComponent::RigidbodyComponent(GameObject* gameObject)
    : BaseComponent(gameObject), m_Velocity(0.0f, 0.0f), m_Force(0.0f, 0.0f), m_Gravity(9.8f), m_TransformComponent(nullptr)
{
}

void dae::RigidbodyComponent::FixedUpdate()
{
    if (m_TransformComponent == nullptr)
        m_TransformComponent = GetGameObject()->GetComponent<TransformComponent>();

    if (m_TransformComponent)
    {
        // Apply gravity force
        m_Force.y += m_Gravity;

        // Integrate velocity
        m_Velocity += m_Force;

        // Update position
        auto pos = m_TransformComponent->GetWorldPosition();
        pos += glm::vec3(m_Velocity, 0.0f) * Settings::fixed_time_step;
        m_TransformComponent->SetWorldPosition(pos.x, pos.y, pos.z);

        // Reset force
        m_Force = glm::vec2(0.0f, 0.0f);

        // Check for collisions
        CheckCollisions();
    }
}

void dae::RigidbodyComponent::ApplyForce(const glm::vec2& force)
{
    m_Force += force;
}

void dae::RigidbodyComponent::SetGravity(float gravity)
{
    m_Gravity = gravity;
}

void dae::RigidbodyComponent::SetTransformComponent(TransformComponent* transformComponent)
{
    m_TransformComponent = transformComponent;
}

void dae::RigidbodyComponent::CheckCollisions()
{
    auto collider = GetGameObject()->GetComponent<ColliderComponent>();
    if (collider)
    {
        for (auto other : SceneManager::GetInstance().GetActiveScene()->GetAllGameObjects())
        {
            if (other == GetGameObject())
                continue;

            auto otherCollider = other->GetComponent<ColliderComponent>();
            if (otherCollider && collider->IsCollidingWith(otherCollider))
            {
                OnCollision.Invoke(GetGameObject(), other);
            }
        }
    }
}
