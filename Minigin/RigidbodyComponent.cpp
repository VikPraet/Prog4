#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "SceneManager.h"
#include "Settings.h"
#include <iostream>

namespace dae
{
    struct GameObjectPairHash
    {
        std::size_t operator()(const std::pair<GameObject*, GameObject*>& pair) const
        {
            auto hash1 = std::hash<GameObject*>{}(pair.first);
            auto hash2 = std::hash<GameObject*>{}(pair.second);
            return hash1 ^ hash2;
        }
    };

    std::unordered_set<std::pair<GameObject*, GameObject*>, GameObjectPairHash> activeCollisions;
}

dae::RigidbodyComponent::RigidbodyComponent(GameObject* gameObject, bool simulateGravity)
    : BaseComponent(gameObject), m_Velocity(0.0f, 0.0f), m_Force(0.0f, 0.0f), m_SimulateGravity(simulateGravity), m_Gravity(Settings::gravity), m_TransformComponent(nullptr), m_ColliderComponent(nullptr)
{
}

void dae::RigidbodyComponent::Update()
{
    if (!m_TransformComponent) m_TransformComponent = GetGameObject()->GetComponent<TransformComponent>();
    if (!m_ColliderComponent) m_ColliderComponent = GetGameObject()->GetComponent<ColliderComponent>();
}

void dae::RigidbodyComponent::FixedUpdate()
{
    if (m_TransformComponent)
    {
        // Apply gravity force
        if (m_SimulateGravity)
        {
            m_Force += m_Gravity * Settings::fixed_time_step;
        }

        // Integrate velocity
        m_Velocity += m_Force;

        // Update position with Continuous Collision Detection (CCD)
        auto startPos = glm::vec2(m_TransformComponent->GetWorldPosition());
        auto endPos = startPos + m_Velocity * Settings::fixed_time_step;

        CheckCollisions(startPos, endPos);

        // Update position
        m_TransformComponent->SetWorldPosition(endPos.x, endPos.y, m_TransformComponent->GetWorldPosition().z);

        // Reset force
        m_Force = glm::vec2(0.0f, 0.0f);
    }
}

void dae::RigidbodyComponent::ApplyForce(const glm::vec2& force)
{
    m_Force += force;
}

void dae::RigidbodyComponent::SetGravity(const glm::vec2& gravity)
{
    m_Gravity = gravity;
}

void dae::RigidbodyComponent::CheckCollisions(const glm::vec2& startPos, const glm::vec2& endPos)
{
    if (m_ColliderComponent)
    {
        std::unordered_set<GameObject*> currentCollisions;
        for (auto* other : SceneManager::GetInstance().GetActiveScene()->GetAllGameObjects())
        {
            if (other == GetGameObject())
                continue;

            const auto otherCollider = other->GetComponent<ColliderComponent>();
            if (otherCollider)
            {
                glm::vec2 hitPoint;
                if (CheckContinuousCollision(startPos, endPos, otherCollider, hitPoint))
                {
                    currentCollisions.insert(other);
                    auto collisionPair = std::minmax(GetGameObject(), other);

                    if (!m_CurrentCollisions.contains(other) && dae::activeCollisions.find(collisionPair) == dae::activeCollisions.end())
                    {
                        bool isTrigger = m_ColliderComponent->IsTrigger() || otherCollider->IsTrigger();
                        if (isTrigger)
                        {
                            OnTriggerEnterEvent.Invoke(GetGameObject(), other);
                            if (auto otherRb = other->GetComponent<RigidbodyComponent>())
                                otherRb->OnTriggerEnterEvent.Invoke(other, GetGameObject());
                        }
                        else
                        {
                            OnCollisionEnterEvent.Invoke(GetGameObject(), other);
                            if (auto otherRb = other->GetComponent<RigidbodyComponent>())
                                otherRb->OnCollisionEnterEvent.Invoke(other, GetGameObject());
                            ResolveCollision(otherCollider); // Resolve collision
                        }
                        dae::activeCollisions.insert(collisionPair);
                    }
                }
            }
        }

        for (auto* other : m_CurrentCollisions)
        {
            if (!currentCollisions.contains(other))
            {
                auto collisionPair = std::minmax(GetGameObject(), other);
                const auto otherCollider = other->GetComponent<ColliderComponent>();
                const bool isTrigger = m_ColliderComponent->IsTrigger() || otherCollider->IsTrigger();
                if (dae::activeCollisions.contains(collisionPair))
                {
                    if (isTrigger)
                    {
                        OnTriggerExitEvent.Invoke(GetGameObject(), other);
                        if (auto otherRb = other->GetComponent<RigidbodyComponent>())
                            otherRb->OnTriggerExitEvent.Invoke(other, GetGameObject());
                    }
                    else
                    {
                        OnCollisionExitEvent.Invoke(GetGameObject(), other);
                        if (auto otherRb = other->GetComponent<RigidbodyComponent>())
                            otherRb->OnCollisionExitEvent.Invoke(other, GetGameObject());
                    }
                    dae::activeCollisions.erase(collisionPair);
                }
            }
        }

        m_CurrentCollisions = std::move(currentCollisions);
    }
}

bool dae::RigidbodyComponent::CheckContinuousCollision(const glm::vec2& startPos, const glm::vec2& endPos, const ColliderComponent* otherCollider, glm::vec2& hitPoint)
{
    auto direction = glm::normalize(endPos - startPos);
    float distance = glm::length(endPos - startPos);

    // Perform a simple bounding box collision test for the purpose of this example
    auto otherPos = glm::vec2(otherCollider->GetGameObject()->GetComponent<TransformComponent>()->GetWorldPosition());
    auto otherSize = otherCollider->GetSize();

    glm::vec2 boxMin = otherPos - 0.5f * otherSize;
    glm::vec2 boxMax = otherPos + 0.5f * otherSize;

    glm::vec2 invDir = 1.0f / direction;
    glm::vec2 t0 = (boxMin - startPos) * invDir;
    glm::vec2 t1 = (boxMax - startPos) * invDir;

    glm::vec2 tmin = glm::min(t0, t1);
    glm::vec2 tmax = glm::max(t0, t1);

    float tEnter = glm::max(tmin.x, tmin.y);
    float tExit = glm::min(tmax.x, tmax.y);

    if (tEnter <= tExit && tEnter < distance)
    {
        hitPoint = startPos + tEnter * direction;
        return true;
    }

    return false;
}

void dae::RigidbodyComponent::ResolveCollision(const ColliderComponent* otherCollider)
{
    auto posA = glm::vec2(m_TransformComponent->GetWorldPosition());
    const auto posB = glm::vec2(otherCollider->GetGameObject()->GetComponent<TransformComponent>()->GetWorldPosition());
    const auto sizeA = m_ColliderComponent->GetSize();
    const auto sizeB = otherCollider->GetSize();

    glm::vec2 correction(0.0f);
    glm::vec2 newVelocity = m_Velocity;

    if (posA.x < posB.x)
    {
        correction.x = (posB.x - (posA.x + sizeA.x));
        if (m_Velocity.x > 0) // If moving towards the right
            newVelocity.x = 0;
    }
    else
    {
        correction.x = ((posB.x + sizeB.x) - posA.x);
        if (m_Velocity.x < 0) // If moving towards the left
            newVelocity.x = 0;
    }

    if (posA.y < posB.y)
    {
        correction.y = (posB.y - (posA.y + sizeA.y));
        if (m_Velocity.y > 0) // If moving upwards
            newVelocity.y = 0;
    }
    else
    {
        correction.y = ((posB.y + sizeB.y) - posA.y);
        if (m_Velocity.y < 0) // If moving downwards
            newVelocity.y = 0;
    }

    if (glm::abs(correction.x) < glm::abs(correction.y))
    {
        posA.x -= correction.x;
    }
    else
    {
        posA.y -= correction.y;
    }

    m_TransformComponent->SetWorldPosition(posA.x, posA.y, m_TransformComponent->GetWorldPosition().z);
    m_Velocity = newVelocity;
}
