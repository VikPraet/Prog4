#pragma once
#include "EnemyAttackBehavior.h"

namespace galaga
{
    class ButterflyAttackBehavior final : public EnemyAttackBehavior
    {
    public:
        ButterflyAttackBehavior(dae::GameObject* gameObject);
        ~ButterflyAttackBehavior() = default;

    private:
        std::vector<glm::vec2> CalculateAttackPath() const override;
    };
}
