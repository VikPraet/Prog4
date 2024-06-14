#pragma once
#include "EnemyAttackBehavior.h"

namespace galaga
{
    class BossAttackBehavior final : public EnemyAttackBehavior
    {
    public:
        BossAttackBehavior(dae::GameObject* gameObject);
        ~BossAttackBehavior() = default;

    private:
        std::vector<glm::vec2> CalculateAttackPath() const override;
        void StartAttacking() override;
        void OnPathComplete() override;
        void OnTractorComplete();

        void SpawnTractorBeam();

        bool m_IsOnTractorPath{ false };
        bool m_IsUsingTractor{ false };
    };
}
