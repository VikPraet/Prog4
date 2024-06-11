#include "Health.h"

#include "GameObject.h"

galaga::Health::Health(dae::GameObject* gameObject, int health)
	: BaseComponent(gameObject), m_Health(health)
{
}

void galaga::Health::Hit(int damage)
{
	m_Health -= damage;

	if(m_Health <= 0 )
	{
		m_Health = 0;
		OnDeath.Invoke();
		GetGameObject()->Destroy();
	}
}
