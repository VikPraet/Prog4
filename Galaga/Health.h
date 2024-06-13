#pragma once
#include "BaseComponent.h"
#include "Event.h"

namespace dae
{
	class GameObject;
}

namespace galaga
{
	class Health final : public dae::BaseComponent
	{
	public:
		Health(dae::GameObject* gameObject, int health = 1);
		~Health() = default;

		void Hit(int damage);
		int GetHealth() const { return m_Health; }

		dae::Event<> OnHit;
		dae::Event<dae::GameObject*> OnDeath;

	private:
		int m_Health;
	};
}
