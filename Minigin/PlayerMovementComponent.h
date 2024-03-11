#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"

class PlayerMovementComponent final : public BaseComponent
{
public:
	PlayerMovementComponent(const std::shared_ptr<dae::GameObject>& gameObject, float speed = 50.f);
	~PlayerMovementComponent() = default;

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

private:
	TransformComponent* m_TransformComponent;
	float m_Speed;
};

