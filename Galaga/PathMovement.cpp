#include "PathMovement.h"
#include "GameTime.h"
#include "Settings.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <glm/gtc/constants.hpp>

galaga::PathMovement::PathMovement(dae::GameObject* gameObject, const std::vector<glm::vec2>& path, float speed, bool rotateAlongPath, bool loopPath)
    : BaseComponent(gameObject), m_TransformComponent(nullptr), m_Path(path), m_Speed(speed), m_CurrentPoint(0), m_PathComplete(false), m_RotateAlongPath(rotateAlongPath), m_LoopPath(loopPath)
{
    if (!m_TransformComponent)
        m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();
}

void galaga::PathMovement::Update()
{
    if (!m_TransformComponent)
        m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();

    if (m_TransformComponent && !m_PathComplete)
    {
        MoveAlongPath();
    }
}

void galaga::PathMovement::SetPath(const std::vector<glm::vec2>& path)
{
    m_Path = path;
    m_PathComplete = false;
    m_CurrentPoint = 0;
}

void galaga::PathMovement::SetWorldSpacePath(const std::vector<glm::vec2>& path)
{
	m_Path.clear();
    for (auto& point : path)
    {
        AddWorldSpacePoint(point);
    }
    m_PathComplete = false;
    m_CurrentPoint = 0;
}

bool galaga::PathMovement::IsPathComplete() const
{
    return m_PathComplete;
}

void galaga::PathMovement::SetRotateAlongPath(bool rotate)
{
    m_RotateAlongPath = rotate;
}

void galaga::PathMovement::SetLoopPath(bool loop)
{
    m_LoopPath = loop;
}

void galaga::PathMovement::StartAtFirstPoint()
{
    if (!m_Path.empty())
    {
        m_CurrentPoint = 0;
        m_TransformComponent->SetWorldPosition(ConvertToWorldCoordinates(m_Path[m_CurrentPoint]).x, ConvertToWorldCoordinates(m_Path[m_CurrentPoint]).y, m_TransformComponent->GetWorldPosition().z);
    }
}

void galaga::PathMovement::AddWorldSpacePoint(const glm::vec2& point)
{
	const float screenWidth = static_cast<float>(dae::Settings::window_width);
	const float screenHeight = static_cast<float>(dae::Settings::window_height);
    glm::vec2 normalizedPoint = glm::vec2(point.x / screenWidth, point.y / screenHeight);
    m_Path.emplace_back(normalizedPoint);
}

void galaga::PathMovement::ResetPath()
{
    m_LoopPath = false;
    m_PathComplete = false;
    m_CurrentPoint = 0;
    m_Path.clear();
}


void galaga::PathMovement::MoveAlongPath()
{
    if (!m_TransformComponent) return;

    if (m_CurrentPoint >= m_Path.size())
    {
        if (m_LoopPath)
        {
            m_CurrentPoint = 0;
        }
        else
        {
            m_PathComplete = true;
            OnPathCompleted.Invoke();
            return;
        }
    }

    auto position = m_TransformComponent->GetWorldPosition();
    const glm::vec2 target = ConvertToWorldCoordinates(m_Path[m_CurrentPoint]);

    glm::vec2 direction = target - glm::vec2(position.x, position.y);
    const float distance = length(direction);

    if (distance < m_Speed * dae::GameTime::GetInstance().GetDeltaTime())
    {
        m_TransformComponent->SetWorldPosition(target.x, target.y, position.z);
        m_CurrentPoint++;
    }
    else
    {
        direction = normalize(direction);
        position.x += direction.x * m_Speed * dae::GameTime::GetInstance().GetDeltaTime();
        position.y += direction.y * m_Speed * dae::GameTime::GetInstance().GetDeltaTime();

        // Update rotation
        if (m_RotateAlongPath)
        {
            const float targetAngle = std::atan2(direction.y, direction.x) * (180.0f / glm::pi<float>()) + 90.0f;
            const float currentAngle = m_TransformComponent->GetRotation();

            // Calculate angle difference
            float angleDifference = targetAngle - currentAngle;

            // Ensure angle difference is within [-180, 180] range
            while (angleDifference < -180.0f)
                angleDifference += 360.0f;
            while (angleDifference > 180.0f)
                angleDifference -= 360.0f;

            // Define lerp speed for rotation
            if (m_CurrentPoint <= 1) m_TransformComponent->SetRotation(targetAngle);
            else
            {
	            constexpr float rotationLerpSpeed = 15.0f;
				const float newAngle = currentAngle + angleDifference * rotationLerpSpeed * dae::GameTime::GetInstance().GetDeltaTime();
				m_TransformComponent->SetRotation(newAngle);
            }
        }

        // Update position
        m_TransformComponent->SetWorldPosition(position.x, position.y, position.z);
    }
}

glm::vec2 galaga::PathMovement::ConvertToWorldCoordinates(const glm::vec2& relativePos)
{
    float x = relativePos.x * dae::Settings::window_width;
    float y = relativePos.y * dae::Settings::window_height;
    return { x, y };
}
