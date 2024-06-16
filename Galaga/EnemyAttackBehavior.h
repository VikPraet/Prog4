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

        bool IsAttacking() const { return m_IsAttacking; }
        bool IsReturning() const { return m_ReturnToFormation; }

        dae::Event<dae::GameObject*> OnAttackCompleted;
        dae::Event<> OnShoot;

    protected:
        virtual void Shoot();
        void CheckReturnToFormation();
        void UpdateReturnPath();
        virtual std::vector<glm::vec2> CalculateAttackPath() const = 0;

        virtual void OnPathComplete();

        bool m_IsAttacking{ false };
        bool m_ReturnToFormation{ false };

        BasicEnemyMovementBehavior* m_MovementBehavior{ nullptr };
        PathMovement* m_PathMovement{ nullptr };
        dae::TransformComponent* m_Transform{ nullptr };
        glm::vec3 m_TargetFormationPosition;

        const float m_PositionTolerance{ 1.0f };

        // Timing variables
        float m_AttackDuration{ 0.0f };
        float m_ShootTimer{ 0.0f };
        int m_ShotsFired{ 0 };

        // Configurable timings
        float m_AttackDelay{ 0.1f };
        float m_ShootInterval{ 0.15f };
        int m_ShotsPerAttack{ 2 };
        float m_FireChance{ 0.5f };
        float m_FiringWindow{ 0.5f };

    private:
        void InitializeComponents();
    };
}
