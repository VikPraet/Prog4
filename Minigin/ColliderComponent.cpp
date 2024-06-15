#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include <iostream>

namespace dae
{
    struct GameObjectPairHash
    {
        std::size_t operator()(const std::pair<GameObject*, GameObject*>& pair) const
        {
            const auto hash1 = std::hash<GameObject*>{}(pair.first);
            const auto hash2 = std::hash<GameObject*>{}(pair.second);
            return hash1 ^ hash2;
        }
    };

    std::unordered_set<std::pair<GameObject*, GameObject*>, GameObjectPairHash> active_overlaps;
}

dae::ColliderComponent::ColliderComponent(GameObject* gameObject, const glm::vec2& size)
    : BaseComponent(gameObject), m_Size(size), m_TransformComponent(nullptr)
{
}

void dae::ColliderComponent::Update()
{
    if (!m_TransformComponent)
        m_TransformComponent = GetGameObject()->GetComponent<TransformComponent>();
}

void dae::ColliderComponent::FixedUpdate()
{
    CheckOverlaps();
}

bool dae::ColliderComponent::IsOverlappingWith(const ColliderComponent* other) const
{
    if (!m_TransformComponent || !other->m_TransformComponent)
        return false;

    const auto posA = glm::vec2(m_TransformComponent->GetWorldPosition()) - m_Size / 2.0f;
    const auto posB = glm::vec2(other->m_TransformComponent->GetWorldPosition()) - other->m_Size / 2.0f;
    const auto sizeA = m_Size;
    const auto sizeB = other->m_Size;

    return (posA.x < posB.x + sizeB.x &&
            posA.x + sizeA.x > posB.x &&
            posA.y < posB.y + sizeB.y &&
            posA.y + sizeA.y > posB.y);
}

void dae::ColliderComponent::CheckOverlaps()
{
    std::unordered_set<GameObject*> currentOverlaps;
    for (auto* other : SceneManager::GetInstance().GetActiveScene()->GetAllGameObjects())
    {
        if (other == GetGameObject() || other->IsMarkedForDestroy() || !other || !other->GetActive())
            continue;

        const auto otherCollider = other->GetComponent<ColliderComponent>();
        if (!otherCollider || !otherCollider->GetActive()) continue;

        if (IsOverlappingWith(otherCollider))
        {
            currentOverlaps.insert(other);
            auto overlapPair = std::minmax(GetGameObject(), other);

            if (!m_CurrentOverlaps.contains(other) && !active_overlaps.contains(overlapPair))
            {
                OnTriggerEnterEvent.Invoke(GetGameObject(), other);
                if (const auto otherColliderComponent = other->GetComponent<ColliderComponent>())
                    otherColliderComponent->OnTriggerEnterEvent.Invoke(other, GetGameObject());

                active_overlaps.insert(overlapPair);
            }
        }
    }

    for (auto* other : m_CurrentOverlaps)
    {
        if (!other || other->IsMarkedForDestroy() || !other->GetActive()) continue;
        if (!currentOverlaps.contains(other))
        {
            auto overlapPair = std::minmax(GetGameObject(), other);
            if (active_overlaps.contains(overlapPair))
            {
                OnTriggerExitEvent.Invoke(GetGameObject(), other);
                if (const auto otherCollider = other->GetComponent<ColliderComponent>())
                {
                    if (!otherCollider->GetActive()) continue;

                    otherCollider->OnTriggerExitEvent.Invoke(other, GetGameObject());
                }
                active_overlaps.erase(overlapPair);
            }
        }
    }

    m_CurrentOverlaps = std::move(currentOverlaps);
}