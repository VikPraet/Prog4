#include "DisplayComponentHealth.h"
#include "GameObject.h"
#include "HealthComponent.h"

galaga::DisplayComponentHealth::DisplayComponentHealth(dae::GameObject* gameObject, HealthComponent* healthComponent)
	: BaseComponent(gameObject)
{
	m_TextComponent = GetGameObject()->GetComponent<dae::TextComponent>();

	if (healthComponent)
	{
		healthComponent->OnHealthChanged().AddListener(this, &DisplayComponentHealth::OnHealthChanged);
	}
}

void galaga::DisplayComponentHealth::OnHealthChanged(float currentHealth)
{
	if(m_TextComponent) m_TextComponent->SetText(m_BaseText + std::to_string(static_cast<int>(currentHealth)));
}