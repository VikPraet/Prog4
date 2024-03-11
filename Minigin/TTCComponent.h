#pragma once
#include "BaseComponent.h"

class TTCComponent final : public BaseComponent
{
public:
	TTCComponent(const std::shared_ptr<dae::GameObject>& gameObject);
	~TTCComponent() = default;

	void Render() const override;
};

