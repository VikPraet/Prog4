#include "ScoreManager.h"

galaga::ScoreManager::ScoreManager(dae::GameObject* gameObject)
	: SingletonComponent(gameObject)
{
}

void galaga::ScoreManager::AddScore(int amount)
{
	m_Score += amount;
	OnScoreChanged.Invoke(m_Score);
}
