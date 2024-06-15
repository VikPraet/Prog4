#pragma once
#include "SingletonComponent.h"
#include <vector>
#include <glm/vec2.hpp>

namespace galaga
{
	class GameMaster : public dae::SingletonComponent<GameMaster>
	{
	public:
		GameMaster(dae::GameObject* gameObject = nullptr);
		~GameMaster() = default;

		void LateUpdate() override;

	private:
		void InitializePlayers();
		void RespawnPlayer(int index);
		void OnPlayerKilled(dae::GameObject* gameObject);

		bool m_PlayersInitialized{ false };
		std::vector<dae::GameObject*> m_Players;
		std::vector<float> m_RespawnTimers;

		float m_PlayerRespawnTime{ 3.0f };
	};
}