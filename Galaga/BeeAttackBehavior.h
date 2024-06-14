#pragma once
#include "EnemyAttackBehavior.h"

namespace galaga
{
    class BeeAttackBehavior final : public EnemyAttackBehavior
    {
    public:
        BeeAttackBehavior(dae::GameObject* gameObject);
        ~BeeAttackBehavior() = default;

    private:
        void Shoot() override;
        std::vector<glm::vec2> CalculateAttackPath() const override;
    };
}
