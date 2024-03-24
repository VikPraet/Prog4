#include "HealthComponent.h"
#include "GameObject.h"

dae::HealthComponent::HealthComponent(const std::shared_ptr<dae::GameObject>& gameObject, float maxLife)
	: BaseComponent(gameObject), Subject(gameObject.get()), m_MaxHealth(maxLife), m_CurrentHealth(maxLife)/*, m_OnHealthChanged(m_CurrentHealth)*/
{
}

void dae::HealthComponent::DecreaseHealth(float lifeAmount)
{
	m_CurrentHealth -= lifeAmount;
	if (m_CurrentHealth < 0) m_CurrentHealth = 0;

	//m_OnHealthChanged.SetData(m_CurrentHealth);
	Invoke(OnHealthChange);
}
