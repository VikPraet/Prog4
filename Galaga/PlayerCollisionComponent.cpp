#include "PlayerCollisionComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Health.h"
#include "LifeTime.h"
#include "RenderComponent.h"
#include "AnimatorComponent.h"
#include "SceneManager.h"

galaga::PlayerCollisionComponent::PlayerCollisionComponent(dae::GameObject* gameObject)
	: BaseComponent(gameObject), m_TransformComponent(nullptr), m_ColliderComponent(nullptr)
{
	if (!m_TransformComponent) m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();

    if (!m_Health)
    {
        m_Health = GetGameObject()->GetComponent<Health>();
        if(m_Health) m_Health->OnDeath.AddListener(this, &PlayerCollisionComponent::OnDeath);
    }

    if (!m_ColliderComponent)
    {
        m_ColliderComponent = GetGameObject()->GetComponent<dae::ColliderComponent>();
        if (m_ColliderComponent) m_ColliderComponent->OnTriggerEnterEvent.AddListener(this, &PlayerCollisionComponent::OnTriggerEnter);
    }
}

void galaga::PlayerCollisionComponent::Update()
{
    if (!m_TransformComponent) m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();

    if (!m_Health)
    {
        m_Health = GetGameObject()->GetComponent<Health>();
        m_Health->OnDeath.AddListener(this, &PlayerCollisionComponent::OnDeath);
    }

    if (!m_ColliderComponent)
    {
        m_ColliderComponent = GetGameObject()->GetComponent<dae::ColliderComponent>();
        if (m_ColliderComponent)
        {
            m_ColliderComponent->OnTriggerEnterEvent.AddListener(this, &PlayerCollisionComponent::OnTriggerEnter);
        }
    }
}

void galaga::PlayerCollisionComponent::OnTriggerEnter([[maybe_unused]] dae::GameObject* self, dae::GameObject* other)
{
	if(other->GetTag() != "enemy" && other->GetTag() != "enemyBullet") return;

    m_Health->Hit(1); 
    if (const auto otherHealth = other->GetComponent<Health>()) otherHealth->Hit(1);
    else other->Destroy();
}

void galaga::PlayerCollisionComponent::OnDeath([[maybe_unused]] dae::GameObject* killedObject)
{
    SpawnExplosion();
}

void galaga::PlayerCollisionComponent::SpawnExplosion() const
{
    // Create a new GameObject for the bullet GameObject
    auto explosionObject = std::make_unique<dae::GameObject>();

    // Add Transform component with texture to the GameObject
    explosionObject->AddComponent<dae::TransformComponent>(explosionObject.get());
    explosionObject->GetComponent<dae::TransformComponent>()->SetScale(2);

    // Add Render component with texture to the GameObject
    explosionObject->AddComponent<dae::RenderComponent>(explosionObject.get());
    explosionObject->GetComponent<dae::RenderComponent>()->SetTexture("galaga-ship-explosion.png");

    // animator
    explosionObject->AddComponent<dae::AnimatorComponent>(explosionObject.get(), 1, 4, 7.f);

    // lifeTime
    explosionObject->AddComponent<LifeTime>(explosionObject.get(), 0.57f);

    // Set the the explosion's position to the enemy's position
    const auto enemyPosition = m_TransformComponent->GetWorldPosition();
    explosionObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(enemyPosition);

    // Add the bullet GameObject to the current scene
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(explosionObject));
}
