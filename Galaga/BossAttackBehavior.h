#pragma once
#include "EnemyAttackBehavior.h"

namespace galaga
{
    class BossAttackBehavior final : public EnemyAttackBehavior
    {
    public:
        BossAttackBehavior(dae::GameObject* gameObject);
        ~BossAttackBehavior() = default;

        void StartAttacking() override;
        void SetHasCaughtPlayer(bool hasCaughtPlayer) { m_HasCaughtPlayer = hasCaughtPlayer; }
        bool IsOnTractorPath() const { return m_IsOnTractorPath; }

    private:
        std::vector<glm::vec2> CalculateAttackPath() const override;
        void OnPathComplete() override;
        void OnTractorComplete();

        void SpawnTractorBeam();

        std::vector<glm::vec2> GenerateStandardPlayerPath(const glm::vec2& enemyPosition, const glm::vec2& playerPosition) const;
        std::vector<glm::vec2> GenerateTractorAttackPath(const glm::vec2& enemyPosition, const glm::vec2& playerPosition) const;

        bool m_IsOnTractorPath{ false };
        bool m_IsUsingTractor{ false };
        bool m_HasCaughtPlayer{ false };
    };

}
