#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace galaga
{
    class Bullet;

    class PlayerAttackBehavior final : public dae::BaseComponent
    {
    public:
        PlayerAttackBehavior(dae::GameObject* gameObject);
        ~PlayerAttackBehavior() = default;

        void Update() override;

        void Shoot();
        void SetFireRate(float rpm);

    private:
        void SpawnBullet() const;

        dae::TransformComponent* m_TransformComponent;
        float m_FireRate{ .5f };
        float m_TimeSinceLastShot{ .0f };
    };
}
