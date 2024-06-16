#include "Health.h"

#include "GameObject.h"
#include "ServiceLocator.h"

galaga::Health::Health(dae::GameObject* gameObject, int health)
	: BaseComponent(gameObject), m_Health(health)
{
}

void galaga::Health::Hit(int damage)
{
	m_Health -= damage;
	OnHit.Invoke();

	if(m_Health <= 0 )
	{
		m_Health = 0;
		OnDeath.Invoke(this->GetGameObject());
		GetGameObject()->Destroy();

		dae::ServiceLocator::GetService<ISoundService>()->PlaySoundEffect("Galaga-kill.wav");
	}
}
