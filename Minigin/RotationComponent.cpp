#include "RotationComponent.h"

#include "GameObject.h"
#include "GameTime.h"
#include "TransformComponent.h"
#include <SDL_stdinc.h>

RotationComponent::RotationComponent(const std::shared_ptr<dae::GameObject>& gameObject, float distanceFromTarget, float rotationSpeed, glm::vec3 rotateAroundPoint)
	: BaseComponent(gameObject), m_RotationSpeed(rotationSpeed), m_DistanceFromTarget(distanceFromTarget), m_RotationPoint(rotateAroundPoint), m_TransformComponent(nullptr)
{
    m_TransformComponent = gameObject->GetComponent<TransformComponent>();
    if (m_TransformComponent)
        m_RotationPoint = m_TransformComponent->GetPosition();
}

void RotationComponent::Update()
{
    if (m_TransformComponent == nullptr)
        return;

    static float elapsedTime = 0;
    elapsedTime += GameTime::GetInstance().GetDeltaTime();

    const float angle = elapsedTime * m_RotationSpeed;
    const glm::vec3 offset = glm::vec3{ std::cos(angle) * m_DistanceFromTarget, std::sin(angle) * m_DistanceFromTarget, 0 };
    const glm::vec3 targetPosition = m_RotationPoint + offset;

    m_TransformComponent->SetPosition(targetPosition.x, targetPosition.y, targetPosition.z);

    if(m_RotateImage)
    {
        // Calculate the orientation angle
        const glm::vec3 direction = m_RotationPoint - targetPosition;
        const float rotationAngle = std::atan2(direction.y, direction.x) * (180.0f / static_cast<float>(M_PI));

        // Set the object's rotation (assuming a function SetRotation exists)
        m_TransformComponent->SetRotation(rotationAngle);
    }
}