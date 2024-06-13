#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include <glm/glm.hpp>

namespace galaga
{
    class Bullet final : public dae::BaseComponent
    {
    public:
        Bullet(dae::GameObject* gameObject, float speed = 150.f, float lifetime = 2.0f, const glm::vec2& direction = glm::vec2(0, -1));
        ~Bullet() = default;

        void Update() override;
        void SetDirection(const glm::vec2& direction);
        void SetSpeed(float speed);

    private:
        dae::TransformComponent* m_TransformComponent;
        glm::vec2 m_Direction;
        float m_Speed;
        float m_Lifetime;

        void UpdateRotation() const;
    };
}
