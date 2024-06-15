#pragma once
#include "BaseComponent.h"

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
		dae::TransformComponent* m_Transform;
		dae::TransformComponent* m_ParentTransform;
	};
}
