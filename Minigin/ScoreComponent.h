#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace galaga
{
	class ScoreComponent final : public dae::BaseComponent, public dae::Subject
	{
	public:
		ScoreComponent(dae::GameObject* gameObject);
		~ScoreComponent() = default;

		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		void IncreaseScore(int amount = 1.f);
		int GetCurrentScore() const { return m_CurrentScore; }

	private:
		int m_CurrentScore{};
	};
}
