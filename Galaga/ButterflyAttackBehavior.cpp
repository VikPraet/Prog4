#include "ButterflyAttackBehavior.h"
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

galaga::ButterflyAttackBehavior::ButterflyAttackBehavior(dae::GameObject* gameObject)
	:EnemyAttackBehavior(gameObject)
{
}

void galaga::ButterflyAttackBehavior::Shoot()
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
    bulletObject->AddComponent<Bullet>(bulletObject.get(), 300.f, 3.f, direction);

    // Add Collider component
    bulletObject->AddComponent<dae::ColliderComponent>(bulletObject.get(), glm::vec2(6.f, 14.f));

    // Set the bullet's initial position to the bee's position
    bulletObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(beePosition);

    // Set the tag to "enemyBullet"
    bulletObject->SetTag("enemyBullet");

    // Add the bullet GameObject to the current scene
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(bulletObject));
}

std::vector<glm::vec2> galaga::ButterflyAttackBehavior::CalculateAttackPath() const
{
    auto playerPosition = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("player")[0]->GetComponent<dae::TransformComponent>()->GetWorldPosition();
    auto enemyPosition = m_Transform->GetWorldPosition();
    std::vector<glm::vec2> path;

    if (enemyPosition.x < playerPosition.x)
    {
        path = {
            // loop at top
            { enemyPosition.x, enemyPosition.y - 25 },
            { enemyPosition.x - 20, enemyPosition.y - 30 },
            { enemyPosition.x - 35, enemyPosition.y - 25 },
            { enemyPosition.x - 35, enemyPosition.y - 10 },
            { enemyPosition.x - 20, enemyPosition.y },
            // curve right
            { enemyPosition.x + 30 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 30 },
            { enemyPosition.x + 48 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 35 },
            { enemyPosition.x + 60 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 45 },
            { enemyPosition.x + 67 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 56 },
            { enemyPosition.x + 72 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 66 },
            { enemyPosition.x + 77 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 75 },
            { enemyPosition.x + 81 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 85 },
            { enemyPosition.x + 84 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 96 },
            { enemyPosition.x + 86 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 106 },
            { enemyPosition.x + 88 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 112 },
            { enemyPosition.x + 88 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 126 },
            { enemyPosition.x + 84 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 137 },
            { enemyPosition.x + 83 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 145 },
            { enemyPosition.x + 80 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 155 },
            { enemyPosition.x + 75 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 165 },
            { enemyPosition.x + 70 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 175 },
            { enemyPosition.x + 62 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 185 },
            { enemyPosition.x + 50 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 200 },
            { enemyPosition.x + 30 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 215 },
            // curve left
            { enemyPosition.x - 48 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 250 },
            { enemyPosition.x - 60 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 260 },
            { enemyPosition.x - 67 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 271 },
            { enemyPosition.x - 72 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 281 },
            { enemyPosition.x - 77 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 290 },
            { enemyPosition.x - 81 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 300 },
            { enemyPosition.x - 84 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 310 },
            { enemyPosition.x - 86 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 312 },
            { enemyPosition.x - 88 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 327 },
            { enemyPosition.x - 88 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 341 },
            // go to edge of screen
            { enemyPosition.x - 88 + abs(playerPosition.x - enemyPosition.x), playerPosition.y + 150 },
            { dae::Settings::window_width - 15 , playerPosition.y + 150 },
            // go to center
            { dae::Settings::window_width - 15 , playerPosition.y - 60 },
            { dae::Settings::window_width - 15 , playerPosition.y - 75 },
            { dae::Settings::window_width - 25 , playerPosition.y - 85 },
            { dae::Settings::window_width - 40 , playerPosition.y - 95 },
            { dae::Settings::window_width - 55 , playerPosition.y - 110 },
            { dae::Settings::window_width / 2 + 150 , playerPosition.y - 140 },
            { dae::Settings::window_width / 2 + 145 , playerPosition.y - 150 },
            { dae::Settings::window_width / 2 + 140 , playerPosition.y - 165 },
            { dae::Settings::window_width / 2 + 130 , playerPosition.y - 180 },
        };
    }
    else
    {
        path = {
            // loop at top
            { enemyPosition.x, enemyPosition.y - 25 },
            { enemyPosition.x + 20, enemyPosition.y - 30 },
            { enemyPosition.x + 35, enemyPosition.y - 25 },
            { enemyPosition.x + 35, enemyPosition.y - 10 },
            { enemyPosition.x + 20, enemyPosition.y },
            // curve right
            { enemyPosition.x - 30 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 30 },
            { enemyPosition.x - 48 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 35 },
            { enemyPosition.x - 60 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 45 },
            { enemyPosition.x - 67 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 56 },
            { enemyPosition.x - 72 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 66 },
            { enemyPosition.x - 77 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 75 },
            { enemyPosition.x - 81 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 85 },
            { enemyPosition.x - 84 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 96 },
            { enemyPosition.x - 86 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 106 },
            { enemyPosition.x - 88 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 112 },
            { enemyPosition.x - 88 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 126 },
            { enemyPosition.x - 84 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 137 },
            { enemyPosition.x - 83 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 145 },
            { enemyPosition.x - 80 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 155 },
            { enemyPosition.x - 75 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 165 },
            { enemyPosition.x - 70 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 175 },
            { enemyPosition.x - 62 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 185 },
            { enemyPosition.x - 50 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 200 },
            { enemyPosition.x - 30 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 215 },
            // curve left
            { enemyPosition.x + 48 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 250 },
            { enemyPosition.x + 60 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 260 },
            { enemyPosition.x + 67 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 271 },
            { enemyPosition.x + 72 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 281 },
            { enemyPosition.x + 77 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 290 },
            { enemyPosition.x + 81 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 300 },
            { enemyPosition.x + 84 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 310 },
            { enemyPosition.x + 86 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 312 },
            { enemyPosition.x + 88 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 327 },
            { enemyPosition.x + 88 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 341 },
            // go to edge of screen
            { enemyPosition.x + 88 - abs(playerPosition.x - enemyPosition.x), playerPosition.y + 150 },
            { 15 , playerPosition.y + 150 },
            // go to center
            { 15 , playerPosition.y - 60 },
            { 15 , playerPosition.y - 75 },
            { 25 , playerPosition.y - 85 },
            { 40 , playerPosition.y - 95 },
            { 55 , playerPosition.y - 110 },
            { dae::Settings::window_width / 2 - 150 , playerPosition.y - 140 },
            { dae::Settings::window_width / 2 - 145 , playerPosition.y - 150 },
            { dae::Settings::window_width / 2 - 140 , playerPosition.y - 165 },
            { dae::Settings::window_width / 2 - 130 , playerPosition.y - 180 },
        };
    }

    return path;
}
