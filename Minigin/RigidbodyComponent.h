#pragma once
#include "BaseComponent.h"
#include "glm/glm.hpp"
#include "Event.h"

namespace dae
{
    class TransformComponent;
    class ColliderComponent;

    class RigidbodyComponent : public BaseComponent
    {
    public:
        RigidbodyComponent(GameObject* gameObject, bool simulateGravity = true);
        void Update() override;
        void FixedUpdate() override;

        void ApplyForce(const glm::vec2& force);
        void SetGravity(const glm::vec2& gravity);

        Event<GameObject*, GameObject*> OnCollisionEnterEvent;
        Event<GameObject*, GameObject*> OnCollisionExitEvent;
        Event<GameObject*, GameObject*> OnTriggerEnterEvent;
        Event<GameObject*, GameObject*> OnTriggerExitEvent;

    private:
        void CheckCollisions(const glm::vec2& startPos, const glm::vec2& endPos);
        void ResolveCollision(const ColliderComponent* otherCollider);
        bool CheckContinuousCollision(const glm::vec2& startPos, const glm::vec2& endPos, const ColliderComponent* otherCollider, glm::vec2& hitPoint);

        glm::vec2 m_Velocity;
        glm::vec2 m_Force;
        glm::vec2 m_Gravity;
        bool m_SimulateGravity;

        TransformComponent* m_TransformComponent;
        ColliderComponent* m_ColliderComponent;
        std::unordered_set<GameObject*> m_CurrentCollisions;
    };
}
