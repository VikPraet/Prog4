#include "PlayerAttackBehavior.h"
#include "GameObject.h"
#include "Bullet.h"
#include "ColliderComponent.h"
#include "ColliderRenderComponent.h"
#include "GameTime.h"
#include "RenderComponent.h"
#include "SceneManager.h"

galaga::PlayerAttackBehavior::PlayerAttackBehavior(dae::GameObject* gameObject)
    : BaseComponent(gameObject), m_TransformComponent(nullptr)
{
}

void galaga::PlayerAttackBehavior::Update()
{
    if (!m_TransformComponent) m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();
    m_TimeSinceLastShot += dae::GameTime::GetInstance().GetDeltaTime();
}

void galaga::PlayerAttackBehavior::Shoot()
{
    if (m_TimeSinceLastShot >= m_FireRate && dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("bullet").size() < 2)
    {
        SpawnBullet();
        m_TimeSinceLastShot = 0.0f;
    }
}

void galaga::PlayerAttackBehavior::SetFireRate(float rpm)
{
    m_FireRate = 60.0f / rpm;
}

void galaga::PlayerAttackBehavior::SpawnBullet() const
{
    // Create a new GameObject for the bullet GameObject
    auto bulletObject = std::make_unique<dae::GameObject>();

    // Add Transform component with texture to the GameObject
    bulletObject->AddComponent<dae::TransformComponent>(bulletObject.get());
    bulletObject->GetComponent<dae::TransformComponent>()->SetScale(2);

    // Add Render component with texture to the GameObject
    bulletObject->AddComponent<dae::RenderComponent>(bulletObject.get());
    bulletObject->GetComponent<dae::RenderComponent>()->SetTexture("fighter-bullet.png");

    // Add Bullet component to the GameObject
    bulletObject->AddComponent<Bullet>(bulletObject.get(), 450.f, 1.1f);

    // Add Collider to game Object
    bulletObject->AddComponent<dae::ColliderComponent>(bulletObject.get(), glm::vec2(6.f, 14.f));
    //bulletObject->AddComponent<dae::ColliderRenderComponent>(bulletObject.get(), glm::vec4(1.f));

    // Set the bullet's initial position to the player's position
    const auto playerPosition = m_TransformComponent->GetWorldPosition();
    bulletObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(playerPosition);

    // Set the tag to bullet
    bulletObject->SetTag("bullet");

    // Add the bullet GameObject to the current scene
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(bulletObject));
}
