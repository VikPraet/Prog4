#include "ButterflyAttackBehavior.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SceneManager.h"
#include "Settings.h"
#include "TransformComponent.h"
#include "GameTime.h"

galaga::ButterflyAttackBehavior::ButterflyAttackBehavior(dae::GameObject* gameObject)
	:EnemyAttackBehavior(gameObject)
{
}

std::vector<glm::vec2> galaga::ButterflyAttackBehavior::CalculateAttackPath() const
{
    const auto players = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("player");
    if (!players.empty())
    {
        const auto player = players[0];
	    const auto playerPosition = player->GetComponent<dae::TransformComponent>()->GetWorldPosition();
        const auto enemyPosition = m_Transform->GetWorldPosition();
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
    return {};
}
