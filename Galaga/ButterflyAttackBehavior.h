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
        void Shoot() override;
        std::vector<glm::vec2> CalculateAttackPath() const override;
    };
}
