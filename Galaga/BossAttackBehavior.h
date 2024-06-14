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
    };
}
