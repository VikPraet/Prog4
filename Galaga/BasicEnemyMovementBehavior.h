#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace galaga
{
    class BasicEnemyMovementBehavior : public dae::BaseComponent
    {
    public:
        BasicEnemyMovementBehavior(dae::GameObject* gameObject, float speed = 50.0f, glm::vec2 startPosition = glm::vec2(0));
        ~BasicEnemyMovementBehavior() = default;

        void Update() override;

        void SetBounds(float minX, float maxX);
        void SetCanMove(bool canMove) { m_CanMove = canMove; }
        void ResetPosition(const glm::vec2& position);
        glm::vec3 GetCurrentPosition() const { return m_Position; }

    private:
        dae::TransformComponent* m_TransformComponent;
        float m_MinX, m_MaxX;
        float m_Speed;
        bool m_MovingRight;
        bool m_CanMove;
        glm::vec3 m_Position;

        void KeepWithinBounds();
    };
}
