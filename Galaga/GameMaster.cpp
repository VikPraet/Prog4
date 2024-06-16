#include "GameMaster.h"

#include "ColliderComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "GameTime.h"
#include "PlayerHealth.h"
#include "PlayerMovementBehavior.h"
#include "RotationComponent.h"
#include "Settings.h"
#include "TransformComponent.h"

galaga::GameMaster::GameMaster(dae::GameObject* gameObject)
	: SingletonComponent(gameObject)
{
}

void galaga::GameMaster::LateUpdate()
{
	if (!m_PlayersInitialized)
	{
		InitializePlayers();
	}

	// Accumulate time
	const float deltaTime = dae::GameTime::GetInstance().GetDeltaTime();

	// Update the respawn timers
	for (int i{}; i < static_cast<int>(m_RespawnTimers.size()); ++i)
	{
		if (m_RespawnTimers[i] > 0)
		{
			m_RespawnTimers[i] -= deltaTime;
			if (m_RespawnTimers[i] <= 0)
			{
				// Respawn the player
				RespawnPlayer(i);
			}
		}
	}
}

void galaga::GameMaster::InitializePlayers()
{
	auto players = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("player");
	for (auto* player : players)
	{
		m_Players.push_back(player);
		m_RespawnTimers.push_back(0.0f);

		player->GetComponent<PlayerHealth>()->OnPlayerDeath.AddListener(this, &GameMaster::OnPlayerKilled);
	}
	m_PlayersInitialized = true;
}

void galaga::GameMaster::RespawnPlayer(int index)
{
	if (index >= 0 && index < static_cast<int>(m_Players.size()))
	{
		dae::GameObject* player = m_Players[index];
		// Reactivate and initialize the player
		player->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), static_cast<float>(dae::Settings::window_height - 60));
		player->SetActive(true);
		player->GetComponent<dae::ColliderComponent>()->SetActive(true);
		player->GetComponent<PlayerMovementBehavior>()->SetActive(true);
		player->SetTag("player");
	}
}

void galaga::GameMaster::OnPlayerKilled(dae::GameObject* gameObject)
{
	if (!m_PlayersInitialized)
	{
		InitializePlayers();
	}

	// Find the index of the player in the current list
	auto it = std::ranges::find(m_Players, gameObject);
	if (it != m_Players.end())
	{
		const int index = static_cast<int>(std::distance(m_Players.begin(), it));
		// Set the respawn timer for this player
		m_RespawnTimers[index] = m_PlayerRespawnTime;
	}

	// Move the player off-screen
	gameObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(-1000, -1000);
	gameObject->GetComponent<dae::TransformComponent>()->SetRotation(0.f);
	gameObject->SetActive(false);
}