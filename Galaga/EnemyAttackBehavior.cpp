#include "EnemyAttackBehavior.h"
#include <cstdlib>
#include <random>
#include <glm/glm.hpp>
#include "Bullet.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Settings.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "RenderComponent.h"

galaga::EnemyAttackBehavior::EnemyAttackBehavior(dae::GameObject* gameObject)
    : BaseComponent(gameObject)
{
    InitializeComponents();
}

void galaga::EnemyAttackBehavior::Update()
{
    InitializeComponents();

    CheckReturnToFormation();

    if (m_IsAttacking)
    {
        m_AttackDuration += dae::GameTime::GetInstance().GetDeltaTime();
        if (m_AttackDuration >= m_AttackDelay && m_AttackDuration <= m_AttackDelay + m_FiringWindow && m_ShotsFired < m_ShotsPerAttack)
        {
            m_ShootTimer += dae::GameTime::GetInstance().GetDeltaTime();

            if (m_ShootTimer >= m_ShootInterval)
            {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(0.0, 1.0);

                if (dis(gen) < m_FireChance)
                {
                    Shoot();
                    m_ShotsFired++;
                }
                m_ShootTimer = 0.0f;
            }
        }
    }
}

void galaga::EnemyAttackBehavior::StartAttacking()
{
    if (m_IsAttacking || m_ReturnToFormation) return;

    m_IsAttacking = true;
    m_AttackDuration = 0.0f;
    m_ShootTimer = 0.0f;
    m_ShotsFired = 0;

    if (m_MovementBehavior) m_MovementBehavior->SetCanMove(false);
    if (m_PathMovement)
    {
        m_PathMovement->SetWorldSpacePath(CalculateAttackPath());
        m_PathMovement->SetSpeed(300.f);
    }
}

void galaga::EnemyAttackBehavior::StopAttacking()
{
    m_IsAttacking = false;
    m_ReturnToFormation = true;
    m_TargetFormationPosition = m_MovementBehavior->GetCurrentPosition();
}

void galaga::EnemyAttackBehavior::Shoot()
{
    const auto& players = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("player");

    if (players.empty()) return;

    auto bulletObject = std::make_unique<dae::GameObject>();

    bulletObject->AddComponent<dae::TransformComponent>(bulletObject.get());
    bulletObject->GetComponent<dae::TransformComponent>()->SetScale(2);

    bulletObject->AddComponent<dae::RenderComponent>(bulletObject.get());
    bulletObject->GetComponent<dae::RenderComponent>()->SetTexture("enemy-bullet.png");

    const auto playerPosition = players[0]->GetComponent<dae::TransformComponent>()->GetWorldPosition();
    const auto enemyPosition = m_Transform->GetWorldPosition();
    glm::vec2 direction = glm::normalize(glm::vec2(playerPosition.x - enemyPosition.x, playerPosition.y - enemyPosition.y));

    bulletObject->AddComponent<Bullet>(bulletObject.get(), 300.f, 3.f, direction);
    bulletObject->AddComponent<dae::ColliderComponent>(bulletObject.get(), glm::vec2(6.f, 14.f));
    bulletObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(enemyPosition);
    bulletObject->SetTag("enemyBullet");

    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(bulletObject));
}

void galaga::EnemyAttackBehavior::CheckReturnToFormation()
{
    if (!m_ReturnToFormation) return;
    UpdateReturnPath();
}

void galaga::EnemyAttackBehavior::UpdateReturnPath()
{
    glm::vec3 currentFormationPosition = m_MovementBehavior->GetCurrentPosition();
    if (glm::length(currentFormationPosition - m_TargetFormationPosition) > m_PositionTolerance)
    {
        if (m_PathMovement)
            m_PathMovement->SetWorldSpacePath({ currentFormationPosition });
        m_TargetFormationPosition = currentFormationPosition;
    }
}

void galaga::EnemyAttackBehavior::OnPathComplete()
{
    if (m_IsAttacking)
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

void galaga::EnemyAttackBehavior::InitializeComponents()
{
    if (!m_Transform) m_Transform = GetGameObject()->GetComponent<dae::TransformComponent>();
    if (!m_MovementBehavior) m_MovementBehavior = GetGameObject()->GetComponent<BasicEnemyMovementBehavior>();
    if (!m_PathMovement)
    {
        m_PathMovement = GetGameObject()->GetComponent<PathMovement>();
        m_PathMovement->OnPathCompleted.AddListener(this, &EnemyAttackBehavior::OnPathComplete);
    }
}
