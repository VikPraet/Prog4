#include "HealthComponent.h"
#include "GameObject.h"

galaga::HealthComponent::HealthComponent(dae::GameObject* gameObject, float maxLife)
	: BaseComponent(gameObject), Subject(gameObject), m_MaxHealth(maxLife), m_CurrentHealth(maxLife)
{
}

void galaga::HealthComponent::DecreaseHealth(float lifeAmount)
{
	m_CurrentHealth -= lifeAmount;
	if (m_CurrentHealth < 0) m_CurrentHealth = 0;

	Invoke(dae::OnHealthChange);
}
