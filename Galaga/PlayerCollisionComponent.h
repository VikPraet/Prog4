#pragma once
#include "BaseComponent.h"



namespace dae
{
	class ColliderComponent;
	class TransformComponent;
}

namespace galaga
{
	class Health;

	class PlayerCollisionComponent final : public dae::BaseComponent
	{
	public:
		PlayerCollisionComponent(dae::GameObject* gameObject);
		~PlayerCollisionComponent() = default;

		void Update() override;

		void OnTriggerEnter([[maybe_unused]] dae::GameObject* self, dae::GameObject* other);
		void OnDeath([[maybe_unused]] dae::GameObject* killedObject);


	private:
		void SpawnExplosion() const;

		dae::ColliderComponent* m_ColliderComponent;
		dae::TransformComponent* m_TransformComponent;
		Health* m_Health;
	};
}
