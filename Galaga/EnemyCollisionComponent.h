#pragma once
#include "GameObject.h"

namespace dae
{
	class TransformComponent;
}

namespace dae
{
	class ColliderComponent;
}

namespace galaga
{
	class EnemyCollisionComponent final : public dae::BaseComponent
	{
	public:
		EnemyCollisionComponent(dae::GameObject* gameObject);
		~EnemyCollisionComponent() = default;

		void Update() override;

		void OnTriggerEnter(dae::GameObject* self, dae::GameObject* other);

	private:
		dae::ColliderComponent* m_ColliderComponent;
		dae::TransformComponent* m_TransformComponent;

		void SpawnExplosion() const;
	};
}
