#include "ScoreComponent.h"

galaga::ScoreComponent::ScoreComponent(dae::GameObject* gameObject)
	: BaseComponent(gameObject), Subject(gameObject)
{
}

void galaga::ScoreComponent::IncreaseScore(int amount)
{
	m_CurrentScore += amount;

	Invoke(dae::OnScoreChange);
}
