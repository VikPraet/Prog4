#include "PathMovement.h"
#include "GameTime.h"
#include "Settings.h"
#include "GameObject.h"
#include <glm/gtc/constants.hpp>


galaga::PathMovement::PathMovement(dae::GameObject* gameObject, const std::vector<glm::vec2>& path, float speed, bool rotateAlongPath)
    : BaseComponent(gameObject), m_TransformComponent(nullptr), m_Path(path), m_Speed(speed), m_CurrentPoint(0), m_PathComplete(false), m_RotateAlongPath(rotateAlongPath)
{
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

bool galaga::PathMovement::IsPathComplete() const
{
    return m_PathComplete;
}

void galaga::PathMovement::SetRotateAlongPath(bool rotate)
{
    m_RotateAlongPath = rotate;
}

void galaga::PathMovement::MoveAlongPath()
{
    if (m_CurrentPoint >= m_Path.size())
    {
        m_PathComplete = true;
        return;
    }

    auto position = m_TransformComponent->GetWorldPosition();
    const glm::vec2 target = ConvertToWorldCoordinates(m_Path[m_CurrentPoint]);

    glm::vec2 direction = target - glm::vec2(position.x, position.y);
    const float distance = glm::length(direction);

    if (distance < m_Speed * dae::GameTime::GetInstance().GetDeltaTime())
    {
        m_TransformComponent->SetWorldPosition(target.x, target.y, position.z);
        m_CurrentPoint++;
    }
    else
    {
        direction = glm::normalize(direction);
        position.x += direction.x * m_Speed * dae::GameTime::GetInstance().GetDeltaTime();
        position.y += direction.y * m_Speed * dae::GameTime::GetInstance().GetDeltaTime();
        m_TransformComponent->SetWorldPosition(position.x, position.y, position.z);

        if (m_RotateAlongPath)
        {
	        const float angle = std::atan2(direction.y, direction.x) * (180.0f / glm::pi<float>());
            m_TransformComponent->SetRotation(angle + 90.0f);
        }
    }
}

glm::vec2 galaga::PathMovement::ConvertToWorldCoordinates(const glm::vec2& relativePos)
{
    float x = relativePos.x * dae::Settings::window_width;
    float y = relativePos.y * dae::Settings::window_height;
    return { x, y };
}
