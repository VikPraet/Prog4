#pragma once
#include "BaseComponent.h"
#include "Event.h"

namespace dae
{
	class GameObject;
}

namespace galaga
{
	class Health : public dae::BaseComponent
	{
	public:
		Health(dae::GameObject* gameObject, int health = 1);
		~Health() = default;

		virtual void Hit(int damage);
		int GetHealth() const { return m_Health; }

		dae::Event<> OnHit;
		dae::Event<dae::GameObject*> OnDeath;

	protected:
		int m_Health;
	};
}
