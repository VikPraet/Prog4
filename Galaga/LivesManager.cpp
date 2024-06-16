#include "LivesManager.h"

galaga::LivesManager::LivesManager(dae::GameObject* gameObject)
	: SingletonComponent(gameObject)
{
}

void galaga::LivesManager::LoseLife(int amount)
{
	m_Lives -= amount;
	OnLivesChanged.Invoke(m_Lives);
}
