#include "EnemyAttackBehavior.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SceneManager.h"
#include "Settings.h"
#include "TransformComponent.h"
#include "GameTime.h"

galaga::EnemyAttackBehavior::EnemyAttackBehavior(dae::GameObject* gameObject)
    : BaseComponent(gameObject)
{
    m_Transform = gameObject->GetComponent<dae::TransformComponent>();
    m_MovementBehavior = gameObject->GetComponent<BasicEnemyMovementBehavior>();
    m_PathMovement = gameObject->GetComponent<PathMovement>();
    if (m_PathMovement)
    {
        m_PathMovement->OnPathCompleted.AddListener(this, &EnemyAttackBehavior::OnPathComplete);
    }
}

void galaga::EnemyAttackBehavior::Update()
{
    if (!m_Transform) m_Transform = GetGameObject()->GetComponent<dae::TransformComponent>();
    if (!m_MovementBehavior) m_MovementBehavior = GetGameObject()->GetComponent<BasicEnemyMovementBehavior>();
    if (!m_PathMovement)
    {
        m_PathMovement = GetGameObject()->GetComponent<PathMovement>();
        m_PathMovement->OnPathCompleted.AddListener(this, &EnemyAttackBehavior::OnPathComplete);
    }

    CheckReturnToFormation();

    if (m_IsAttacking)
    {
        m_AttackDuration += dae::GameTime::GetInstance().GetDeltaTime();
        if (m_AttackDuration >= m_AttackDelay && m_AttackDuration <= m_AttackDelay + m_FiringWindow && m_ShotsFired < m_ShotsPerAttack)
        {
            m_ShootTimer += dae::GameTime::GetInstance().GetDeltaTime();

            if (m_ShootTimer >= m_ShootInterval)
            {
                if (std::rand() / static_cast<float>(RAND_MAX) < m_FireChance)
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

void galaga::EnemyAttackBehavior::CheckReturnToFormation()
{
    if (!m_ReturnToFormation) return;
    UpdateReturnPath();
}

void galaga::EnemyAttackBehavior::UpdateReturnPath()
{
    glm::vec3 currentFormationPosition = m_MovementBehavior->GetCurrentPosition();
    if (std::abs(currentFormationPosition.x - m_TargetFormationPosition.x) > m_PositionTolerance ||
        std::abs(currentFormationPosition.y - m_TargetFormationPosition.y) > m_PositionTolerance ||
        std::abs(currentFormationPosition.z - m_TargetFormationPosition.z) > m_PositionTolerance)
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
