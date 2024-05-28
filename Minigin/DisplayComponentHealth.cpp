#include "DisplayComponentHealth.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

galaga::DisplayComponentHealth::DisplayComponentHealth(dae::GameObject* gameObject)
	: BaseComponent(gameObject), Observer()
{
	m_TextComponent = GetGameObject()->GetComponent<dae::TextComponent>();
}

void galaga::DisplayComponentHealth::Notify(const dae::Event& event, dae::Subject* subject)
{
	switch (event)
	{
	case dae::Initialize:
	case dae::OnHealthChange:
		UpdateHealthDisplay(subject);
		break;
	}
}

void galaga::DisplayComponentHealth::UpdateHealthDisplay(dae::Subject* subject)
{
	const auto healthComponent = subject->GetParent()->GetComponent<HealthComponent>();
	const int currentLife = static_cast<int>(healthComponent->GetCurrentHealth());
	m_TextComponent->SetText(m_BaseText + std::to_string(currentLife));
}