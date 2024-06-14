#include "RotationComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "TransformComponent.h"
#include <glm/glm.hpp>
#include <cmath>
#include <glm/ext/scalar_constants.hpp>

dae::RotationComponent::RotationComponent(dae::GameObject* gameObject, float distanceFromTarget, float rotationSpeed, glm::vec3 rotateAroundPoint)
    : BaseComponent(gameObject), m_RotationSpeed(rotationSpeed), m_DistanceFromTarget(distanceFromTarget), m_RotationPoint(rotateAroundPoint), m_TransformComponent(nullptr)
{
    m_TransformComponent = gameObject->GetComponent<TransformComponent>();
    if (m_TransformComponent && !m_RotateAroundSelf)
        m_RotationPoint = m_TransformComponent->GetWorldPosition();
}

void dae::RotationComponent::Update()
{
    if (m_TransformComponent == nullptr)
        return;

    static float elapsedTime = 0;
    elapsedTime += GameTime::GetInstance().GetDeltaTime();

    if (m_RotateAroundSelf)
    {
        // Calculate the rotation angle directly
        const float angle = std::fmod(elapsedTime * m_RotationSpeed, 360.0f);
        m_TransformComponent->SetRotation(angle);
    }
    else
    {
        // Rotate around a specified point
        const float angle = elapsedTime * m_RotationSpeed;
        const glm::vec3 offset = glm::vec3{ std::cos(angle) * m_DistanceFromTarget, std::sin(angle) * m_DistanceFromTarget, 0 };
        const glm::vec3 targetPosition = m_RotationPoint + offset;

        m_TransformComponent->SetLocalPosition(targetPosition);

        if (m_RotateImage)
        {
            // Calculate the orientation angle
            const glm::vec3 direction = m_RotationPoint - targetPosition;
            float rotationAngle = std::atan2(direction.y, direction.x) * (180.0f / glm::pi<float>());
            if (m_RotationSpeed < 0) rotationAngle += 180;

            // Set the object's rotation
            m_TransformComponent->SetRotation(rotationAngle);
        }
    }
}
