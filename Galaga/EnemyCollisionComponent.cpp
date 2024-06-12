#include "EnemyCollisionComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "AnimatorComponent.h"
#include "LifeTime.h"
#include "SceneManager.h"
#include "Health.h"

galaga::EnemyCollisionComponent::EnemyCollisionComponent(dae::GameObject* gameObject)
	: BaseComponent(gameObject), m_ColliderComponent(nullptr), m_TransformComponent(nullptr), m_Health(nullptr)
{
}

void galaga::EnemyCollisionComponent::Update()
{
    if(!m_TransformComponent) m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();
    if (!m_Health)
    {
        m_Health = GetGameObject()->GetComponent<Health>();
        m_Health->OnDeath.AddListener(this, &EnemyCollisionComponent::OnDeath);
    }

    if (!m_ColliderComponent)
    {
        m_ColliderComponent = GetGameObject()->GetComponent<dae::ColliderComponent>();
        if (m_ColliderComponent)
        {
            m_ColliderComponent->OnTriggerEnterEvent.AddListener(this, &EnemyCollisionComponent::OnTriggerEnter);
        }
    }
}

void galaga::EnemyCollisionComponent::OnTriggerEnter([[maybe_unused]] dae::GameObject* self, dae::GameObject* other)
{
    if (other->GetTag() != "bullet") return;
    other->Destroy();

    if (m_Health) m_Health->Hit(1);
}

void galaga::EnemyCollisionComponent::OnDeath()
{
    SpawnExplosion();
}

void galaga::EnemyCollisionComponent::SpawnExplosion() const
{
    // Create a new GameObject for the bullet GameObject
    auto explosionObject = std::make_unique<dae::GameObject>();

    // Add Transform component with texture to the GameObject
    explosionObject->AddComponent<dae::TransformComponent>(explosionObject.get());
    explosionObject->GetComponent<dae::TransformComponent>()->SetScale(2);

    // Add Render component with texture to the GameObject
    explosionObject->AddComponent<dae::RenderComponent>(explosionObject.get());
    explosionObject->GetComponent<dae::RenderComponent>()->SetTexture("galaga-explosion.png");

    // animator
    explosionObject->AddComponent<dae::AnimatorComponent>(explosionObject.get(), 1, 5, 15.f);

    // lifeTime
    explosionObject->AddComponent<LifeTime>(explosionObject.get(), 0.3f);

    // Set the the explosion's position to the enemy's position
    const auto enemyPosition = m_TransformComponent->GetWorldPosition();
    explosionObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(enemyPosition);

    // Add the bullet GameObject to the current scene
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(explosionObject));
}
