#include "TransformComponent.h"

TransformComponent::TransformComponent(const std::shared_ptr<dae::GameObject>& gameObject)
	: BaseComponent(gameObject), m_position(0.0f, 0.0f, 0.0f), m_rotation(0.0f), m_scale(1.0f, 1.0f, 1.0f)
{
}

void TransformComponent::SetPosition(float x, float y, float z)
{
	m_position = glm::vec3(x, y, z);
}

void TransformComponent::SetRotation(float angle)
{
	m_rotation = angle;
}

void TransformComponent::SetScale(float scaleX, float scaleY, float scaleZ)
{
	m_scale = glm::vec3(scaleX, scaleY, scaleZ);
}
