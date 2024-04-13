#include "DisplayComponentHealth.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

dae::DisplayComponentHealth::DisplayComponentHealth(dae::GameObject* gameObject)
	: BaseComponent(gameObject), Observer()
{
	m_TextComponent = GetGameObject()->GetComponent<TextComponent>();
}

void dae::DisplayComponentHealth::Notify(const Event& event, Subject* subject)
{
	switch (event)
	{
	case Initialize:
	case OnHealthChange:
		UpdateHealthDisplay(subject);
		break;
	}
}

void dae::DisplayComponentHealth::UpdateHealthDisplay(Subject* subject)
{
	const auto healthComponent = subject->GetParent()->GetComponent<HealthComponent>();
	const int currentLife = static_cast<int>(healthComponent->GetCurrentHealth());
	m_TextComponent->SetText(m_BaseText + std::to_string(currentLife));
}