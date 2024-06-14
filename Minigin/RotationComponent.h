#pragma once
#include "BaseComponent.h"
#include <glm/vec3.hpp>

namespace dae
{
    class TransformComponent;

    class RotationComponent final : public BaseComponent
    {
    public:
        RotationComponent(GameObject* gameObject, float distanceFromTarget = 0.f, float rotationSpeed = 1.0f, glm::vec3 rotateAroundPoint = { 0,0,0 });
        ~RotationComponent() = default;

        void Update() override;

        void SetRotateImage(bool rotateImage) { m_RotateImage = rotateImage; }
        void SetRotateAroundSelf(bool rotateAroundSelf) { m_RotateAroundSelf = rotateAroundSelf; }
        void SetRotatetionSpeed(float rotationSpeed) { m_RotationSpeed = rotationSpeed; }

    private:
        float m_RotationSpeed{};
        float m_DistanceFromTarget{};
        glm::vec3 m_RotationPoint{};
        TransformComponent* m_TransformComponent;

        bool m_RotateImage{ false };
        bool m_RotateAroundSelf{ false };
    };
}
