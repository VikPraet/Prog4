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
	class Health;

	class EnemyCollisionComponent final : public dae::BaseComponent
	{
	public:
		EnemyCollisionComponent(dae::GameObject* gameObject);
		~EnemyCollisionComponent() = default;

		void Update() override;

		void OnTriggerEnter([[maybe_unused]] dae::GameObject* self, dae::GameObject* other);
		void OnDeath();

	private:
		dae::ColliderComponent* m_ColliderComponent;
		dae::TransformComponent* m_TransformComponent;
		Health* m_Health;

		void SpawnExplosion() const;
	};
}
