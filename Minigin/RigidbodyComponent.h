#pragma once

#include "BaseComponent.h"
#include "CollisionEvent.h"
#include <glm/glm.hpp>

namespace dae
{
    class TransformComponent;

    class RigidbodyComponent : public BaseComponent
    {
    public:
        RigidbodyComponent(GameObject* gameObject);
        ~RigidbodyComponent() = default;

        void FixedUpdate() override;
        void ApplyForce(const glm::vec2& force);
        void SetGravity(float gravity);

        void SetTransformComponent(TransformComponent* transformComponent);

        CollisionEvent OnCollision;

    private:
        glm::vec2 m_Velocity;
        glm::vec2 m_Force;
        float m_Gravity;
        TransformComponent* m_TransformComponent;

        void CheckCollisions();
    };
}
