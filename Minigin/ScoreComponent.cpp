#include "ScoreComponent.h"

galaga::ScoreComponent::ScoreComponent(dae::GameObject* gameObject)
	: BaseComponent(gameObject)
{
	m_ScoreChangedEvent.Invoke(m_CurrentScore);
}

void galaga::ScoreComponent::IncreaseScore(int amount)
{
	m_CurrentScore += amount;

	m_ScoreChangedEvent.Invoke(m_CurrentScore);
}
