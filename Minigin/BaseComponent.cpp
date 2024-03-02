#include "BaseComponent.h"

BaseComponent::BaseComponent(const std::shared_ptr<dae::GameObject>& gameObject) :
	m_GameObject{gameObject}
{
}
