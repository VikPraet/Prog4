#include "TTCComponent.h"

#include <future>

#include "TrashTheCache.h"


TTCComponent::TTCComponent(const std::shared_ptr<dae::GameObject>& gameObject)
	: BaseComponent(gameObject)
{
}

void TTCComponent::Render() const
{
	ttc::ShowIntGraphWindow();
	ttc::ShowGOGraphWindow();
}

