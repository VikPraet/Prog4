#pragma once
#include "BaseComponent.h"

namespace dae
{
	class AnimatorComponent;
	class ColliderComponent;
}

namespace galaga
{
	class TractorBeam final : public dae::BaseComponent
	{
	public:
		TractorBeam(dae::GameObject* gameObject);
		~TractorBeam() = default;

		void Update() override;

		void OnTriggerEnter(dae::GameObject* self, dae::GameObject* other);

		dae::Event<> OnTractorCompleted;
	private:
		void OnDestroy();
		void OnPathComplete();
		void SpawnEnemyShip();

		dae::ColliderComponent* m_ColliderComponent;
		dae::AnimatorComponent* m_AnimatorComponent;

		bool m_IsActive{ false };
		bool m_IsRetracting{ false };
		float m_ActiveTime{ 2.f };
		float m_AccuTime{ 0.f };

		dae::GameObject* m_Player;
	};
}
