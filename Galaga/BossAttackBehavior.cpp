#include "BossAttackBehavior.h"
#include <cstdlib>
#include <random>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SceneManager.h"
#include "Settings.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "RenderComponent.h"
#include "AnimatorComponent.h"
#include "ColliderComponent.h"
#include "TractorBeam.h"

galaga::BossAttackBehavior::BossAttackBehavior(dae::GameObject* gameObject)
    : EnemyAttackBehavior(gameObject)
{
}

std::vector<glm::vec2> galaga::BossAttackBehavior::CalculateAttackPath() const
{
    const auto players = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("player");
    if (!players.empty())
    {
        const auto player = players[0];
        const auto playerPosition = player->GetComponent<dae::TransformComponent>()->GetWorldPosition();
        const auto enemyPosition = m_Transform->GetWorldPosition();

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        if (m_HasCaughtPlayer)
        {
            return GenerateCaughtPlayerPath(enemyPosition, playerPosition);
        }
        else
        {
            if (dis(gen) < 0.5)
            {
                return GenerateCaughtPlayerPath(enemyPosition, playerPosition);
            }
            else
            {
                return GenerateStandardAttackPath(enemyPosition, playerPosition);
            }
        }
    }
    return {};
}

void galaga::BossAttackBehavior::StartAttacking()
{
    if (m_IsAttacking || m_ReturnToFormation) return;

    m_IsAttacking = true;
    m_AttackDuration = 0.0f;
    m_ShootTimer = 0.0f;
    m_ShotsFired = 0;
    m_IsOnTractorPath = false;

    if (m_MovementBehavior) m_MovementBehavior->SetCanMove(false);
    if (m_PathMovement)
    {
	    const auto path = CalculateAttackPath();
        m_PathMovement->SetWorldSpacePath(path);
        m_PathMovement->SetSpeed(300.f);
        if (path.size() == 6)
        {
            m_IsOnTractorPath = true;
        }
    }
}


void galaga::BossAttackBehavior::OnPathComplete()
{
    if(m_IsOnTractorPath)
    {
        m_IsUsingTractor = true;
        SpawnTractorBeam();
    }
	else if (m_IsAttacking)
    {
        StopAttacking();
    }
    else if (m_ReturnToFormation)
    {
        m_ReturnToFormation = false;
        m_MovementBehavior->SetCanMove(true);
        m_Transform->SetRotation(0.f);
        OnAttackCompleted.Invoke(this->GetGameObject());
    }
}

void galaga::BossAttackBehavior::OnTractorComplete()
{
    StopAttacking();
    m_IsOnTractorPath = false;
    m_IsUsingTractor = false;
}

void galaga::BossAttackBehavior::SpawnTractorBeam()
{
    auto tractorBeam = std::make_unique<dae::GameObject>();

    tractorBeam->AddComponent<dae::TransformComponent>(tractorBeam.get());
    tractorBeam->GetComponent<dae::TransformComponent>()->SetScale(2);

    tractorBeam->AddComponent<dae::RenderComponent>(tractorBeam.get());
    tractorBeam->GetComponent<dae::RenderComponent>()->SetTexture("tractor-beam.png");

    tractorBeam->AddComponent<dae::AnimatorComponent>(tractorBeam.get(), 2, 9, 9.f);

    tractorBeam->AddComponent<TractorBeam>(tractorBeam.get());
    tractorBeam->GetComponent<TractorBeam>()->OnTractorCompleted.AddListener(this, &BossAttackBehavior::OnTractorComplete);

    tractorBeam->SetParent(GetGameObject());
    tractorBeam->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 100);

    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(tractorBeam));
}

std::vector<glm::vec2> galaga::BossAttackBehavior::GenerateCaughtPlayerPath(const glm::vec2& enemyPosition, const glm::vec2& playerPosition) const
{
    if (enemyPosition.x < playerPosition.x)
    {
        return {
            { enemyPosition.x, enemyPosition.y - 25 },
            { enemyPosition.x - 20, enemyPosition.y - 30 },
            { enemyPosition.x - 35, enemyPosition.y - 25 },
            { enemyPosition.x - 35, enemyPosition.y - 10 },
            { enemyPosition.x - 20, enemyPosition.y },
            { enemyPosition.x + 20 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 30 },
            { enemyPosition.x + 30 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 54 },
            { enemyPosition.x + 35 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 77 },
            { enemyPosition.x + 40 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 100 },
            { enemyPosition.x + 46 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 130 },
            { enemyPosition.x + 55 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 170 },
            { enemyPosition.x + 63 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 193 },
            { enemyPosition.x + 72 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 209 },
            { enemyPosition.x + 90 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 229 },
            { enemyPosition.x + 106 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 243 },
            { enemyPosition.x + 122 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 252 },
            { enemyPosition.x + 135 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 256 },
            { enemyPosition.x + 146 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 255 },
            { enemyPosition.x + 157 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 251 },
            { enemyPosition.x + 170 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 242 },
            { enemyPosition.x + 178 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 229 },
            { enemyPosition.x + 184 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 215 },
            { enemyPosition.x + 187 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 198 },
            { enemyPosition.x + 186 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 180 },
            { enemyPosition.x + 182 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 167 },
            { enemyPosition.x + 177 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 157 },
            { enemyPosition.x + 170 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 148 },
            { enemyPosition.x + 163 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 142 },
            { enemyPosition.x + 153 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 137 },
            { enemyPosition.x + 143 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 134 },
            { enemyPosition.x + 132 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 135 },
            { enemyPosition.x + 124 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 138 },
            { enemyPosition.x + 116 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 143 },
            { enemyPosition.x + 109 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 149 },
            { enemyPosition.x + 103 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 156 },
            { enemyPosition.x + 98 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 165 },
            { enemyPosition.x + 94 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 177 },
            { enemyPosition.x + 91 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 194 },
            { enemyPosition.x + 93 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 212 },
            { enemyPosition.x + 98 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 237 },
            { enemyPosition.x + 99 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 253 },
            { enemyPosition.x + 100 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 269 },
            { enemyPosition.x + 95 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 284 },
            { enemyPosition.x + 85 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 294 },
            { enemyPosition.x + 73 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 298 },
            { enemyPosition.x + 60 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 300 },
            { enemyPosition.x + 44 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 300 },
            { enemyPosition.x - 40 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 300 },
            { enemyPosition.x - 58 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 304 },
            { enemyPosition.x - 72 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 314 },
            { enemyPosition.x - 79 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 332 },
            { enemyPosition.x - 81 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 353 },
            { enemyPosition.x - 80 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 371 },
            { enemyPosition.x - 74 + abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 390 },
            { enemyPosition.x - 74 + abs(playerPosition.x - enemyPosition.x), playerPosition.y + 130 },
            { -40, playerPosition.y + 130 },
            { -40 , -40 },
            { dae::Settings::window_width / 2 , -40 }
        };
    }
    else
    {
        return {
            { enemyPosition.x, enemyPosition.y - 25 },
            { enemyPosition.x + 20, enemyPosition.y - 30 },
            { enemyPosition.x + 35, enemyPosition.y - 25 },
            { enemyPosition.x + 35, enemyPosition.y - 10 },
            { enemyPosition.x + 20, enemyPosition.y },
            { enemyPosition.x - 20 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 30 },
            { enemyPosition.x - 30 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 54 },
            { enemyPosition.x - 35 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 77 },
            { enemyPosition.x - 40 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 100 },
            { enemyPosition.x - 46 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 130 },
            { enemyPosition.x - 55 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 170 },
            { enemyPosition.x - 63 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 193 },
            { enemyPosition.x - 72 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 209 },
            { enemyPosition.x - 90 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 229 },
            { enemyPosition.x - 106 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 243 },
            { enemyPosition.x - 122 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 252 },
            { enemyPosition.x - 135 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 256 },
            { enemyPosition.x - 146 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 255 },
            { enemyPosition.x - 157 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 251 },
            { enemyPosition.x - 170 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 242 },
            { enemyPosition.x - 178 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 229 },
            { enemyPosition.x - 184 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 215 },
            { enemyPosition.x - 187 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 198 },
            { enemyPosition.x - 186 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 180 },
            { enemyPosition.x - 182 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 167 },
            { enemyPosition.x - 177 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 157 },
            { enemyPosition.x - 170 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 148 },
            { enemyPosition.x - 163 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 142 },
            { enemyPosition.x - 153 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 137 },
            { enemyPosition.x - 143 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 134 },
            { enemyPosition.x - 132 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 135 },
            { enemyPosition.x - 124 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 138 },
            { enemyPosition.x - 116 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 143 },
            { enemyPosition.x - 109 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 149 },
            { enemyPosition.x - 103 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 156 },
            { enemyPosition.x - 98 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 165 },
            { enemyPosition.x - 94 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 177 },
            { enemyPosition.x - 91 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 194 },
            { enemyPosition.x - 93 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 212 },
            { enemyPosition.x - 98 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 237 },
            { enemyPosition.x - 99 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 253 },
            { enemyPosition.x - 100 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 269 },
            { enemyPosition.x - 95 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 284 },
            { enemyPosition.x - 85 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 294 },
            { enemyPosition.x - 73 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 298 },
            { enemyPosition.x - 60 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 300 },
            { enemyPosition.x - 44 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 300 },
            { enemyPosition.x + 40 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 300 },
            { enemyPosition.x + 58 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 304 },
            { enemyPosition.x + 72 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 314 },
            { enemyPosition.x + 79 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 332 },
            { enemyPosition.x + 81 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 353 },
            { enemyPosition.x + 80 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 371 },
            { enemyPosition.x + 74 - abs(playerPosition.x - enemyPosition.x), enemyPosition.y + 390 },
            { enemyPosition.x + 74 - abs(playerPosition.x - enemyPosition.x), playerPosition.y + 130 },
            { dae::Settings::window_width + 40, playerPosition.y + 130 },
            { dae::Settings::window_width + 40 , -40 },
            { dae::Settings::window_width / 2 , -40 }
        };
    }
}

std::vector<glm::vec2> galaga::BossAttackBehavior::GenerateStandardAttackPath(const glm::vec2& enemyPosition, const glm::vec2& playerPosition) const
{
    return {
        { enemyPosition.x, enemyPosition.y - 25 },
        { enemyPosition.x + 20, enemyPosition.y - 30 },
        { enemyPosition.x + 35, enemyPosition.y - 25 },
        { enemyPosition.x + 35, enemyPosition.y - 10 },
        { playerPosition.x , playerPosition.y - 123 },
        { playerPosition.x , playerPosition.y - 125 }
    };
}