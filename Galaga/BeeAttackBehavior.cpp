#include "BeeAttackBehavior.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include "Bullet.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Settings.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "RenderComponent.h"

galaga::BeeAttackBehavior::BeeAttackBehavior(dae::GameObject* gameObject)
    : EnemyAttackBehavior(gameObject)
{
}

void galaga::BeeAttackBehavior::Shoot()
{
    // Create a new GameObject for the bullet
    auto bulletObject = std::make_unique<dae::GameObject>();

    // Add Transform component with scale
    bulletObject->AddComponent<dae::TransformComponent>(bulletObject.get());
    bulletObject->GetComponent<dae::TransformComponent>()->SetScale(2);

    // Add Render component with texture
    bulletObject->AddComponent<dae::RenderComponent>(bulletObject.get());
    bulletObject->GetComponent<dae::RenderComponent>()->SetTexture("enemy-bullet.png");

    // Calculate direction towards the player
    const auto playerPosition = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("player")[0]->GetComponent<dae::TransformComponent>()->GetWorldPosition();
    const auto beePosition = m_Transform->GetWorldPosition();
    glm::vec2 direction = normalize(glm::vec2(playerPosition.x - beePosition.x, playerPosition.y - beePosition.y));

    // Add Bullet component with the calculated direction
    bulletObject->AddComponent<Bullet>(bulletObject.get(), 400.f, 2.f, direction);

    // Add Collider component
    bulletObject->AddComponent<dae::ColliderComponent>(bulletObject.get(), glm::vec2(6.f, 14.f));

    // Set the bullet's initial position to the bee's position
    bulletObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(beePosition);

    // Set the tag to "enemyBullet"
    bulletObject->SetTag("enemyBullet");

    // Add the bullet GameObject to the current scene
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(bulletObject));
}

std::vector<glm::vec2> galaga::BeeAttackBehavior::CalculateAttackPath() const
{
    auto playerPosition = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("player")[0]->GetComponent<dae::TransformComponent>()->GetWorldPosition();
    auto enemyPosition = m_Transform->GetWorldPosition();
    std::vector<glm::vec2> path;

    if (enemyPosition.x < playerPosition.x)
    {
        path = {
            { enemyPosition.x, enemyPosition.y - 25 },
            { enemyPosition.x - 20, enemyPosition.y - 30 },
            { enemyPosition.x - 35, enemyPosition.y - 25 },
            { enemyPosition.x - 35, enemyPosition.y - 10 },
            { enemyPosition.x - 20, enemyPosition.y },
            { enemyPosition.x + 20 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 30 },
            { enemyPosition.x + 40 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 80 },
            { enemyPosition.x + 40 + abs(playerPosition.x - enemyPosition.x), playerPosition.y - 100 },
            { playerPosition.x - 120, playerPosition.y + 50 },
            { playerPosition.x - 130, playerPosition.y + 55 },
            { playerPosition.x - 150, playerPosition.y + 55 },
            { playerPosition.x - 170, playerPosition.y + 50 },
            { playerPosition.x - 195, playerPosition.y + 40 },
            { playerPosition.x - 215, playerPosition.y + 30 },
            { playerPosition.x - 235, playerPosition.y },
            { playerPosition.x - 255, playerPosition.y - 50 },
            { playerPosition.x - 255, playerPosition.y - 80 },
            { playerPosition.x - 235, playerPosition.y - 110 }
        };
    }
    else
    {
        path = {
            { enemyPosition.x, enemyPosition.y - 25 },
            { enemyPosition.x + 20, enemyPosition.y - 30 },
            { enemyPosition.x + 35, enemyPosition.y - 25 },
            { enemyPosition.x + 35, enemyPosition.y - 10 },
            { enemyPosition.x + 20, enemyPosition.y },
            { enemyPosition.x - 20 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 30 },
            { enemyPosition.x - 40 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 80 },
            { enemyPosition.x - 40 - abs(playerPosition.x - enemyPosition.x), playerPosition.y - 100 },
            { playerPosition.x + 120, playerPosition.y + 50 },
            { playerPosition.x + 130, playerPosition.y + 55 },
            { playerPosition.x + 150, playerPosition.y + 55 },
            { playerPosition.x + 170, playerPosition.y + 50 },
            { playerPosition.x + 195, playerPosition.y + 40 },
            { playerPosition.x + 215, playerPosition.y + 30 },
            { playerPosition.x + 235, playerPosition.y },
            { playerPosition.x + 255, playerPosition.y - 50 },
            { playerPosition.x + 255, playerPosition.y - 80 },
            { playerPosition.x + 235, playerPosition.y - 110 }
        };
    }

    return path;
}
