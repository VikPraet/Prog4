#include "TTCComponent.h"

#include <future>

#include "TrashTheCache.h"


TTCComponent::TTCComponent(dae::GameObject* gameObject)
	: BaseComponent(gameObject)
{
}

void TTCComponent::Render() const
{
	ttc::ShowIntGraphWindow();
	ttc::ShowGOGraphWindow();
}
