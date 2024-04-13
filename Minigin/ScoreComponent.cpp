#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(dae::GameObject* gameObject)
	: BaseComponent(gameObject), Subject(gameObject)
{
}

void dae::ScoreComponent::IncreaseScore(int amount)
{
	m_CurrentScore += amount;

	Invoke(OnScoreChange);
}
