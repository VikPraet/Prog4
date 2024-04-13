#pragma once
#include "BaseComponent.h"

class TTCComponent final : public BaseComponent
{
public:
	TTCComponent(dae::GameObject* gameObject);
	~TTCComponent() = default;

	void Render() const override;
};

