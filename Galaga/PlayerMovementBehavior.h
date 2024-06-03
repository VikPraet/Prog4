#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace galaga
{
    class PlayerMovementBehavior final : public dae::BaseComponent
    {
    public:
        PlayerMovementBehavior(dae::GameObject* gameObject);
        ~PlayerMovementBehavior() = default;

        void Update() override;

        void Move(const glm::vec2& direction);
        void SetBounds(float minX, float maxX);
        void SetSpeed(float speed);

    private:
        dae::TransformComponent* m_TransformComponent;
        float m_MinX, m_MaxX;
        float m_Speed;
        glm::vec2 m_Direction;

        void KeepWithinBounds();
    };
}
