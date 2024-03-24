#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(const std::shared_ptr<dae::GameObject>& gameObject)
	: BaseComponent(gameObject), Subject(gameObject.get())
{
}

void dae::ScoreComponent::IncreaseScore(int amount)
{
	m_CurrentScore += amount;

	Invoke(OnScoreChange);
}
