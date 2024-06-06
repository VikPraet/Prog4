#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace galaga
{
    class BasicEnemyMovementBehavior : public dae::BaseComponent
    {
    public:
        BasicEnemyMovementBehavior(dae::GameObject* gameObject, float speed = 50.0f);
        ~BasicEnemyMovementBehavior() = default;

        void Update() override;

        void SetBounds(float minX, float maxX);

    private:
        dae::TransformComponent* m_TransformComponent;
        float m_MinX, m_MaxX;
        float m_Speed;
        bool m_MovingRight;

        void KeepWithinBounds();
    };
}
