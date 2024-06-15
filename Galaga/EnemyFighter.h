#pragma once
#include "BaseComponent.h"
#include "PathMovement.h"
#include "RotationComponent.h"

namespace dae
{
	class TransformComponent;
}

namespace galaga
{
	class EnemyFighter final : public dae::BaseComponent 
	{
	public:
		EnemyFighter(dae::GameObject* gameObject);
		~EnemyFighter() = default;
	
		void Update() override;
	
	private:
		void OnPathCompleted();
		void Shoot();

		bool m_HasSubscribedToEvent{ false };

		dae::TransformComponent* m_Transform;
		dae::TransformComponent* m_ParentTransform;

		PathMovement* m_PathMovement;
		dae::RotationComponent* m_RotationComponent;
	};
}
