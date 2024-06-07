#include "BaseComponent.h"
#include "GameObject.h"

dae::BaseComponent::BaseComponent(GameObject* gameObject) :
	m_GameObject{gameObject}
{
	gameObject->OnComponentAdded.AddListener(this, &BaseComponent::OnComponentAdded);
}

void dae::BaseComponent::Destroy()
{
	m_IsActive = false;
	m_MarkedForDestroy = true;
}
