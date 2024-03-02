#include "TransformComponent.h"

#include "GameObject.h"

TransformComponent::TransformComponent(const std::shared_ptr<dae::GameObject>& gameObject)
	: BaseComponent(gameObject), m_WorldPosition(0.0f, 0.0f, 0.0f), m_LocalPosition(0.0f, 0.0f, 0.0f), m_rotation(0.0f), m_scale(1.0f, 1.0f, 1.0f)
{
}

const glm::vec3& TransformComponent::GetWorldPosition()
{
	if (m_PositionDirty)
		UpdateWorldPosition();

	return m_WorldPosition;
}

void TransformComponent::SetWorldPosition(float x, float y, float z)
{
	if (GetGameObject()->GetParent() == nullptr)
		SetLocalPosition(x, y, z);
	else
	{
		const glm::vec3 parentPosition = GetGameObject()->GetParent()->GetComponent<TransformComponent>()->GetWorldPosition();
		SetLocalPosition(x - parentPosition.x, y - parentPosition.y, z - parentPosition.z);
	}
}

void TransformComponent::SetWorldPosition(glm::vec3 worldPosition)
{
	SetWorldPosition(worldPosition.x, worldPosition.y, worldPosition.z);
}

void TransformComponent::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	SetPositionDirty();
}

void TransformComponent::SetLocalPosition(glm::vec3 localPosition)
{
	SetLocalPosition(localPosition.x, localPosition.y, localPosition.z);
}

void TransformComponent::SetRotation(float angle)
{
	m_rotation = angle;
}

void TransformComponent::SetScale(float scaleX, float scaleY, float scaleZ)
{
	m_scale = glm::vec3(scaleX, scaleY, scaleZ);
}

void TransformComponent::SetScale(float scale)
{
	SetScale(scale, scale, 1);
}

void TransformComponent::UpdateWorldPosition()
{
	if (GetGameObject()->GetParent() == nullptr)
		m_WorldPosition = m_LocalPosition;
	else
		m_WorldPosition = m_LocalPosition + GetGameObject()->GetParent()->GetComponent<TransformComponent>()->GetWorldPosition();

	m_PositionDirty = false;
}

void TransformComponent::SetPositionDirty()
{
	m_PositionDirty = true;

	for (auto* childPtr : GetGameObject()->GetChildren())
	{
		childPtr->GetComponent<TransformComponent>()->SetPositionDirty();
	}
}
