#include "BeeAttackBehavior.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SceneManager.h"
#include "Settings.h"
#include "TransformComponent.h"
#include "GameTime.h"

galaga::BeeAttackBehavior::BeeAttackBehavior(dae::GameObject* gameObject)
    : EnemyAttackBehavior(gameObject)
{
}

std::vector<glm::vec2> galaga::BeeAttackBehavior::CalculateAttackPath() const
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
    return {};
}
