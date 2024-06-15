#pragma once
#include "EnemyAttackBehavior.h"

namespace galaga
{
    class BossAttackBehavior final : public EnemyAttackBehavior
    {
    public:
        BossAttackBehavior(dae::GameObject* gameObject);
        ~BossAttackBehavior() = default;

        void SetHasCaughtPlayer(bool hasCaughtPlayer) { m_HasCaughtPlayer = hasCaughtPlayer; }
    private:
        std::vector<glm::vec2> CalculateAttackPath() const override;
        void StartAttacking() override;
        void OnPathComplete() override;
        void OnTractorComplete();

        void SpawnTractorBeam();

        std::vector<glm::vec2> GenerateCaughtPlayerPath(const glm::vec2& enemyPosition, const glm::vec2& playerPosition) const;
        std::vector<glm::vec2> GenerateStandardAttackPath(const glm::vec2& enemyPosition, const glm::vec2& playerPosition) const;

        bool m_IsOnTractorPath{ false };
        bool m_IsUsingTractor{ false };
        bool m_HasCaughtPlayer{ false };
    };

}
