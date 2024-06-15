#pragma once
#include "Health.h"

namespace galaga
{
	class PlayerHealth final : public Health
	{
	public:
		PlayerHealth(dae::GameObject* gameObject);
		~PlayerHealth() = default;

		void Hit(int damage) override;

		dae::Event<dae::GameObject*> OnPlayerDeath;
	};
}
