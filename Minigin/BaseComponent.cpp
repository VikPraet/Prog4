#include "BaseComponent.h"

BaseComponent::BaseComponent(const std::shared_ptr<dae::GameObject>& gameObject) :
	m_GameObject{gameObject}
{
}

//void BaseComponent::SetPosition(float x, float y, float z)
//{
//	m_transform.SetPosition(x, y, z);
//}
