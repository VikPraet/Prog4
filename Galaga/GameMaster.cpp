#include "GameMaster.h"

#include "ColliderComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "GameTime.h"
#include "InputManager.h"
#include "PlayerHealth.h"
#include "PlayerMovementBehavior.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Settings.h"
#include "TextComponent.h"
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

	if (!m_GameOver && m_PlayerExtraLives <= 0)
	{
		m_GameOver = true;
		m_IsTimeScaling = true;
		m_TimeScaleTransitionTime = 0.0f;
	}

	// Handle gradual time scale reduction
	if (m_IsTimeScaling)
	{
		m_TimeScaleTransitionTime += deltaTime;
		float t = m_TimeScaleTransitionTime / m_TimeScaleTransitionDuration;
		if (t >= 1.0f)
		{
			t = 1.0f;
			m_IsTimeScaling = false;
			InitializeAndScreen();
		}
		const float newTimeScale = 1.0f - t;
		dae::GameTime::GetInstance().SetTimeScale(newTimeScale);
	}
}

void galaga::GameMaster::InitializePlayers()
{
	const auto& players = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("player");
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
	if (m_PlayerExtraLives <= 0) return;
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
		--m_PlayerExtraLives;
	}

	// Move the player off-screen
	gameObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(-1000, -1000);
	gameObject->GetComponent<dae::TransformComponent>()->SetRotation(0.f);
	gameObject->SetActive(false);
}

void galaga::GameMaster::InitializeAndScreen()
{
	if(!m_HasInitializedEndScreen)
	{
		const auto& scene = dae::SceneManager::GetInstance().GetActiveScene();

		// results text
		auto resultsText = std::make_unique<dae::GameObject>();

		resultsText->AddComponent<dae::TransformComponent>(resultsText.get());
		resultsText->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), static_cast<float>(dae::Settings::window_height / 2) - 30);

		resultsText->AddComponent<dae::RenderComponent>(resultsText.get());

		auto font = dae::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 17);
		resultsText->AddComponent<dae::TextComponent>(resultsText.get(), std::move(font));
		resultsText->GetComponent<dae::TextComponent>()->SetColor({ 255, 0 ,0 ,255 });
		resultsText->GetComponent<dae::TextComponent>()->SetText("-- RESULTS --");

		// shots fired text
		auto shotsFiredText = std::make_unique<dae::GameObject>();

		shotsFiredText->AddComponent<dae::TransformComponent>(shotsFiredText.get());
		shotsFiredText->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), static_cast<float>(dae::Settings::window_height / 2));

		shotsFiredText->AddComponent<dae::RenderComponent>(shotsFiredText.get());

		font = dae::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 15);
		shotsFiredText->AddComponent<dae::TextComponent>(shotsFiredText.get(), std::move(font));
		shotsFiredText->GetComponent<dae::TextComponent>()->SetColor({ 255, 255 ,0 ,255 });
		std::string text = "SHOTS FIRED: " + std::to_string(m_PlayerFiredShots);
		shotsFiredText->GetComponent<dae::TextComponent>()->SetText(text);

		// number of hits text
		auto numberOfHits = std::make_unique<dae::GameObject>();

		numberOfHits->AddComponent<dae::TransformComponent>(numberOfHits.get());
		numberOfHits->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), static_cast<float>(dae::Settings::window_height / 2) + 20);

		numberOfHits->AddComponent<dae::RenderComponent>(numberOfHits.get());

		font = dae::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 15);
		numberOfHits->AddComponent<dae::TextComponent>(numberOfHits.get(), std::move(font));
		numberOfHits->GetComponent<dae::TextComponent>()->SetColor({ 255, 255 ,0 ,255 });
		text = "NUMBER OF HITS: " + std::to_string(m_KilledEnemies);
		numberOfHits->GetComponent<dae::TextComponent>()->SetText(text);

		// hit miss ratio text
		auto HitMissRatio = std::make_unique<dae::GameObject>();

		HitMissRatio->AddComponent<dae::TransformComponent>(HitMissRatio.get());
		HitMissRatio->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), static_cast<float>(dae::Settings::window_height / 2) + 40);

		HitMissRatio->AddComponent<dae::RenderComponent>(HitMissRatio.get());

		font = dae::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 15);
		HitMissRatio->AddComponent<dae::TextComponent>(HitMissRatio.get(), std::move(font));
		HitMissRatio->GetComponent<dae::TextComponent>()->SetColor({ 255, 255 ,0 ,255 });
		int ratio{};
		if (m_PlayerFiredShots > 0) ratio = static_cast<int>(static_cast<float>(m_KilledEnemies) / static_cast<float>(m_PlayerFiredShots) * 100.f);
		text = "HIT MISS RATIO: %" + std::to_string(ratio);
		HitMissRatio->GetComponent<dae::TextComponent>()->SetText(text);

		scene->Add(std::move(resultsText));
		scene->Add(std::move(shotsFiredText));
		scene->Add(std::move(numberOfHits));
		scene->Add(std::move(HitMissRatio));

		m_HasInitializedEndScreen = true;
	}
}
