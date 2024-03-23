#include "BaseComponent.h"

BaseComponent::BaseComponent(const std::shared_ptr<dae::GameObject>& gameObject) :
	m_GameObject{gameObject}
{
}

void BaseComponent::Destroy()
{
	m_IsActive = false;
	m_MarkedForDestroy = true;
}
