#include "EnemyFighter.h"

#include "GameObject.h"
#include "TransformComponent.h"

galaga::EnemyFighter::EnemyFighter(dae::GameObject* gameObject)
	: BaseComponent(gameObject), m_Transform(nullptr), m_ParentTransform(nullptr)
{
	if (!m_Transform) m_Transform = gameObject->GetComponent<dae::TransformComponent>();
	if (!m_ParentTransform)
	{
		if (const auto parent = gameObject->GetParent()) m_Transform = parent->GetComponent<dae::TransformComponent>();
	}
}

void galaga::EnemyFighter::Update()
{
	if (!m_Transform) m_Transform = GetGameObject()->GetComponent<dae::TransformComponent>();
	if (!m_ParentTransform)
	{
		if (const auto parent = GetGameObject()->GetParent()) m_ParentTransform = parent->GetComponent<dae::TransformComponent>();
	}

	// set rotation to rotation of parent
	if(m_ParentTransform && m_Transform) m_Transform->SetRotation(m_ParentTransform->GetRotation());
}
