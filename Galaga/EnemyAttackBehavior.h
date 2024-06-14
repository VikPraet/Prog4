#pragma once
#include "BaseComponent.h"
#include "BasicEnemyMovementBehavior.h"
#include "PathMovement.h"

namespace galaga
{
    class EnemyAttackBehavior : public dae::BaseComponent
    {
    public:
        EnemyAttackBehavior(dae::GameObject* gameObject);
        virtual ~EnemyAttackBehavior() = default;

        virtual void Update() override;
        virtual void StartAttacking();
        virtual void StopAttacking();

        void SetAttackDelay(float delay) { m_AttackDelay = delay; }
        void SetShootInterval(float interval) { m_ShootInterval = interval; }
        void SetShotsPerAttack(int shots) { m_ShotsPerAttack = shots; }
        void SetFireChance(float chance) { m_FireChance = chance; }

        dae::Event<dae::GameObject*> OnAttackCompleted;

    protected:
        virtual void Shoot() = 0;
        void CheckReturnToFormation();
        void UpdateReturnPath();
        virtual std::vector<glm::vec2> CalculateAttackPath() const = 0;

        void OnPathComplete();

        bool m_IsAttacking{ false };
        bool m_ReturnToFormation{ false };

        BasicEnemyMovementBehavior* m_MovementBehavior;
        PathMovement* m_PathMovement;

        dae::TransformComponent* m_Transform;
        glm::vec3 m_TargetFormationPosition;

        const float m_PositionTolerance{ 1.0f };

        // Timing variables
        float m_AttackDuration{ 0.0f };
        float m_ShootTimer{ 0.0f };
        int m_ShotsFired{ 0 };

        // Configurable timings
        float m_AttackDelay{ 0.15f };
        float m_ShootInterval{ 0.15f };
        int m_ShotsPerAttack{ 1 };
        float m_FireChance{ 0.45f };
        float m_FiringWindow{ 0.4f };
    };
}
