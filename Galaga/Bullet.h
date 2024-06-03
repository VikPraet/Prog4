#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace galaga
{
    class Bullet final : public dae::BaseComponent
    {
    public:
        Bullet(dae::GameObject* gameObject, float speed = 150.f, float lifetime = 2.0f);
        ~Bullet() = default;

        void Update() override;
        void SetDirection(const glm::vec2& direction);
        void SetSpeed(float speed);

    private:
        dae::TransformComponent* m_TransformComponent;
        glm::vec2 m_Direction;
        float m_Speed;
        float m_Lifetime;
    };
}
