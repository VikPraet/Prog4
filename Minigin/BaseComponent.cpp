#include "BaseComponent.h"

dae::BaseComponent::BaseComponent(GameObject* gameObject) :
	m_GameObject{gameObject}
{
}

void dae::BaseComponent::Destroy()
{
	m_IsActive = false;
	m_MarkedForDestroy = true;
}
