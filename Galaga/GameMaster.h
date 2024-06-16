#pragma once
#include "SingletonComponent.h"
#include <vector>

namespace galaga
{
	class GameMaster : public dae::SingletonComponent<GameMaster>
	{
	public:
		GameMaster(dae::GameObject* gameObject = nullptr);
		~GameMaster() = default;

		void LateUpdate() override;

		void IncrementKilledEnemies() { ++m_KilledEnemies; }
		void IncrementPlayerShots() { ++m_PlayerFiredShots; }

	private:
		void InitializePlayers();
		void RespawnPlayer(int index);
		void OnPlayerKilled(dae::GameObject* gameObject);

		void InitializeAndScreen();

		bool m_PlayersInitialized{ false };
		std::vector<dae::GameObject*> m_Players;
		std::vector<float> m_RespawnTimers;
		float m_PlayerRespawnTime{ 3.5f };

		static inline int m_KilledEnemies{};
		static inline int m_PlayerFiredShots{};

		bool m_GameOver{ false };
		bool m_HasInitializedEndScreen{ false };

		bool m_IsTimeScaling{ false };
		float m_TimeScaleTransitionDuration{ 1.3f };
		float m_TimeScaleTransitionTime{ 0.0f };
	};
}