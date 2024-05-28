#pragma once
#include "BaseComponent.h"
#include "Event.h"

namespace galaga
{
	class HealthComponent final : public dae::BaseComponent
	{
	public:
		HealthComponent(dae::GameObject* gameObject, float maxLife);
		~HealthComponent() = default;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void DecreaseHealth(float lifeAmount = 1.f);
		float GetCurrentHealth() const { return m_CurrentHealth; }

		dae::Event<float>& OnHealthChanged() { return m_HealthChangedEvent; }

	private:
		float m_MaxHealth{};
		float m_CurrentHealth{};
		dae::Event<float> m_HealthChangedEvent;
	};
}