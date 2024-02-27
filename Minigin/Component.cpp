#include "Component.h"

Component::Component(const std::shared_ptr<dae::GameObject>& gameObject) :
	m_GameObject{gameObject}
{
}

void Component::SetPosition(float x, float y, float z)
{
	m_transform.SetPosition(x, y, z);
}
