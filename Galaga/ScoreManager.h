#pragma once
#include "SingletonComponent.h"

namespace galaga
{
	class ScoreManager final : public dae::SingletonComponent<ScoreManager>
	{
	public:
		ScoreManager(dae::GameObject* gameObject = nullptr);
		~ScoreManager() = default;

		void AddScore(int amount);

		dae::Event<int> OnScoreChanged;

	private:
		int m_Score{};
	};
}
